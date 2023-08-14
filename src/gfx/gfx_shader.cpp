#include "gfx_shader.hpp"
using namespace gfx;

#include <map>
#include <array>
#include <sstream>
#include <cstring>
using namespace std;


using namespace ut;


#define EXCEPTION(...) ( ShaderException(PRINTER(__VA_ARGS__)) )


static char const* get_type_name(GLenum type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER  : return "vertex";
        case GL_FRAGMENT_SHADER: return "fragment";
        case GL_GEOMETRY_SHADER: return "geometry";
        default:break;
    }
    assert(!"invalid shader type");
};

static std::string get_shader_info_log(GLuint id)
{
    GLint length;

    // assigns length with length of information log
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    std::string infoLog;

    if (length > 0)
    {
        infoLog.resize(length);

        // returns the information log for a shader object
        glGetShaderInfoLog(id, length, &length, infoLog.data());
        infoLog.resize(length);
    }

    return infoLog;
}

static string get_program_info_log(GLuint id)
{
    GLint length;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length); // assigns length with length of information log
    std::string info_log;

    if (length > 0)
    {
        info_log.resize(length);
        glGetProgramInfoLog(id, length, &length, info_log.data());	// returns the information log for a shader object
        info_log.resize(length);
    }

    return info_log;
}

struct UniformInfo{ GLint loc; GLint size; GLenum type; };
static map<string, UniformInfo> get_program_uniform_info(GLuint program_id)
{
    map<string, UniformInfo> m;

    int active_uniform_count = 0;
    glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &active_uniform_count);


    array<char,512> buf;
    for (int i = 0; i < active_uniform_count; ++i)
    {
        UniformInfo ui;
        glGetActiveUniform(program_id, i, buf.size(),
            nullptr, &ui.size, &ui.type, buf.data());
        ui.loc = glGetUniformLocation(program_id, buf.data());

        assert(i == ui.loc);

        m.emplace(string{buf.data()}, ui);
    }
    return m;
}

static bool get_compile_status(GLuint id)
{
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    return res == GL_TRUE;
}

static bool get_link_status(GLuint id)
{
    int res;
    glGetProgramiv(id, GL_LINK_STATUS, &res);
    return res == GL_TRUE;
}

ShaderProgram::ShaderProgram() :
    m_loaded    {false},
    m_program   {0},
    m_shaderv   {0},
    m_shaderf   {0},
    m_shaderg   {0}
{}

string ShaderProgram::infov() const
{ assert(m_loaded); return get_shader_info_log(m_shaderv); }

string ShaderProgram::infof() const
{ assert(m_loaded); return get_shader_info_log(m_shaderf); }

string ShaderProgram::infog() const
{ assert(m_loaded); return get_shader_info_log(m_shaderg); }

void ShaderProgram::use() const
{
    glUseProgram(m_program);
}

bool ShaderProgram::tryBindAttribute(GLint loc, Attribute& attribute) const
{
    while (glGetError() != GL_NO_ERROR);
    glBindAttribLocation(m_program, (GLuint)loc, attribute.name);
    if (glGetError() == GL_NO_ERROR)
    {
        attribute.loc = loc;
        return true;
    }
    return false;
}

void ShaderProgram::getUniform(Uniform& uniform) const
{
    if (GLint loc = glGetUniformLocation(m_program, uniform.name.c_str()); loc < 0)
    {
        throw EXCEPTION("Failed to get uniform: %s", uniform.name.c_str());
    }
    else
    {
        uniform.loc = loc;
    }
}

void ShaderProgram::load(
    char const* sourcev,
    char const* sourcef,
    char const* sourceg,
    Uniform** uniforms,
    size_t uniforms_count)
{
    assert(sourcev != nullptr);
    assert(sourcef != nullptr);

    GLuint program = 0;
    GLuint shaderv = 0;
    GLuint shaderf = 0;
    GLuint shaderg = 0;

    bool errp = false;
    bool errv = false;
    bool errf = false;
    bool errg = false;

    bool hasg = sourceg != nullptr;

    try
    {
        //
        // Create
        //

        program = glCreateProgram();
        if (program == 0)
            throw EXCEPTION("Failed to create opengl shader program");

        shaderv = glCreateShader(GL_VERTEX_SHADER);
        if (shaderv == 0)
            throw EXCEPTION("Failed to create opengl vertex shader");

        shaderf = glCreateShader(GL_FRAGMENT_SHADER);
        if (shaderf == 0)
            throw EXCEPTION("Failed to create opengl fragment shader");

        if (hasg)
        {
            shaderg = glCreateShader(GL_GEOMETRY_SHADER);
            if (shaderg == 0)
                throw EXCEPTION("Failed to create opengl geometry shader");
        }

        //
        // Compile & Link
        //

        glShaderSource(shaderv, 1, &sourcev, nullptr);
        glCompileShader(shaderv);
        glAttachShader(program, shaderv);


        glShaderSource(shaderf, 1, &sourcef, nullptr);
        glCompileShader(shaderf);
        glAttachShader(program, shaderf);

        if (hasg)
        {
            glShaderSource(shaderg, 1, &sourceg, nullptr);
            glCompileShader(shaderg);
            glAttachShader(program, shaderg);
        }

        glLinkProgram(program);

        //
        // Collect and throw errors
        //

        errp = !get_link_status(program);
        errv = !get_compile_status(shaderv);
        errf = !get_compile_status(shaderf);

        if (hasg)
        {
            errg = !get_compile_status(shaderg);
        }

        if (errp || errv || errf || errg)
        {
            ShaderException ex;
            ostringstream ss;

            if (errv)
            {
                string s = get_shader_info_log(shaderv);
                ss << PRINTER("Failed to compile opengl vertex shader:\n    ") << s;
                ex.err_vertex = s;
            }

            if (errf)
            {
                string s = get_shader_info_log(shaderf);
                ss << PRINTER("Failed to compile opengl fragment shader:\n    ") << s;
                ex.err_fragment = s;
            }

            if (errg)
            {
                string s = get_shader_info_log(shaderg);
                ss << PRINTER("Failed to compile opengl geometry shader:\n    ") << s;
                ex.err_geometry = s;
            }

            if (errp)
            {
                string s = get_program_info_log(program);
                ss << PRINTER("Failed to link opengl shader program:\n    ") << s;
                ex.err_program = s;
            }

            ex.message = ss.str();
            throw ex;
        }

        //
        // Get Uniforms
        //

        if (uniforms != nullptr && uniforms_count > 0)
        {
            auto info = get_program_uniform_info(program);


            vector<GLint> locs(uniforms_count);
            for (size_t i = 0; i < uniforms_count; ++i)
            {
//                GLint loc = glGetUniformLocation(program, name);
//                if (loc < 0)
//                    throw exception("Failed to get uniform: {}", name);
//                locs[i] = loc;

                Uniform* u = uniforms[i];
                if (auto it = info.find(u->name); it != info.end())
                {

                    u->loc = it->second.loc;
                    if ((GLenum)u->gl_type != it->second.type)
                        throw EXCEPTION("Uniform type mismatch: %s type='%s'", u->name.c_str(), u->glTypeName());
                }
                else
                {
                    throw EXCEPTION("Failed to get uniform: %s", u->name.c_str());
                }
            }

//            for (size_t i = 0; i < uniforms_count; ++i)
//            {
//                uniforms[i]->loc = locs[i];
//            }
        }

        //
        // Replace
        //

        unload();
        m_program = program;
        m_shaderv = shaderv;
        m_shaderf = shaderf;
        m_shaderg = shaderg;
        m_loaded  = true;
    }
    catch (exception const& ex)
    {
        glDeleteShader(shaderv);
        glDeleteShader(shaderf);
        glDeleteShader(shaderg);
        glDeleteShader(program);
        throw ex;
    }
}

void ShaderProgram::unload()
{
    if (m_loaded)
    {
        glDeleteShader(m_shaderv);
        glDeleteShader(m_shaderf);
        glDeleteShader(m_shaderg);
        glDeleteShader(m_program);

        m_program  = 0;
        m_shaderv  = 0;
        m_shaderf  = 0;
        m_shaderg  = 0;
        m_loaded   = false;
    }
}
