#pragma once

#include "gfx_variable.hpp"
#include "gfx_attribute.hpp"
#include "gfx_uniform.hpp"

#include <ut/string.hpp>
#include <exception>
#include <optional>

namespace gfx
{
    class ShaderException : public std::exception
    {
    public:
        using err_type = std::optional<std::string>;


        err_type err_vertex;
        err_type err_fragment;
        err_type err_geometry;
        err_type err_program;

        std::string message;

        ShaderException()
        {}

        ShaderException(ut::strparam s)
            : message{s.str()}
        {}

        inline char const* what() const noexcept override
        { return message.c_str(); }
    };

    class ShaderProgram
    {
    public:
        ShaderProgram();

        inline bool valid() const
        { return m_loaded; }

        inline bool hasg() const
        { return m_loaded && m_shaderg != 0; }

        std::string infov() const;
        std::string infof() const;
        std::string infog() const;

        void use() const;

        bool tryBindAttribute(GLint loc, Attribute& attribute) const;
        void getUniform(Uniform& uniform) const;

        void load(
            char    const* sourcev,
            char    const* sourcef,
            char    const* sourceg,
            Uniform** uniforms,
            size_t    uniforms_count);
        void unload();

    private:
        bool   m_loaded;
        GLuint m_program;

        GLuint m_shaderv;
        GLuint m_shaderf;
        GLuint m_shaderg;
    };
}
