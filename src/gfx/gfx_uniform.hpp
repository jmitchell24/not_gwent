#pragma once

#include "gfx_variable.hpp"

#include <ut/math.hpp>
#include "raylib.h"
#include "raymath.h"

#define __op__(...) inline void operator()(__VA_ARGS__) const

namespace gfx
{
    struct Uniform : public GLSLVariable
    {
        std::string name        ;
        GLint       loc         = -1;

        inline bool valid() const { return loc != -1; }
    };

    struct Sample2D : public Uniform
    {
        __op__(GLint v) { glUniform1i(loc, v); }
    };

#define DECL(__s__, __v__, __st__, __gl__) \
    struct __s__ : public Uniform \
    { \
        __op__(__st__ v) { glUniform1##__gl__(loc, v); } \
    }; \
    struct __v__##2 : public Uniform \
    { \
        __op__(__st__* v)                               { glUniform2##__gl__##v(loc, 1, v); } \
        __op__(__st__ x, __st__ y)                      { glUniform2##__gl__   (loc, x, y); } \
        __op__(ut::vec2x<__st__> const& v)              { glUniform2##__gl__##v(loc, 1, v.data()); } \
    }; \
    struct __v__##3 : public Uniform \
    { \
        __op__(__st__* v)                               { glUniform3##__gl__##v(loc, 1, v); } \
        __op__(__st__ x, __st__ y, __st__ z)            { glUniform3##__gl__   (loc, x, y, z); } \
        __op__(ut::vec3x<__st__> const& v)              { glUniform3##__gl__##v(loc, 1, v.data()); } \
    }; \
    struct __v__##4 : public Uniform \
    { \
        __op__(__st__* v)                               { glUniform4##__gl__##v(loc, 1, v); } \
        __op__(__st__ x, __st__ y, __st__ z, __st__ w)  { glUniform4##__gl__   (loc, x, y, z, w); } \
        __op__(ut::vec4x<__st__> const& v)              { glUniform4##__gl__##v(loc, 1, v.data()); } \
    };

    DECL(Float  , Vec   , float   , f)
    DECL(Int    , IVec  , int     , i)
    DECL(UInt   , UVec  , unsigned, ui)
    DECL(Bool   , BVec  , int     , i)
#undef DECL

    struct Matrix : public Uniform
    {
        __op__(::Matrix const& mat) { glUniformMatrix4fv(loc, 1, false, MatrixToFloat(mat)); }
    };

//#define DECL(__v__, __st__, __vt__, __gl__) \
//    struct __v__##2 : public Uniform \
//    { \
//        __op__(__st__* v)          { glUniformMatrix2##__gl__##v(loc, 1, GL_FALSE, v); } \
//        __op__(__vt__##2 const& v) { glUniformMatrix2##__gl__##v(loc, 1, GL_FALSE, glm::value_ptr(v)); } \
//    }; \
//    struct __v__##3 : public Uniform \
//    { \
//        __op__(__st__* v)          { glUniformMatrix3##__gl__##v(loc, 1, GL_FALSE, v); } \
//        __op__(__vt__##3 const& v) { glUniformMatrix3##__gl__##v(loc, 1, GL_FALSE, glm::value_ptr(v)); } \
//    }; \
//    struct __v__##4 : public Uniform \
//    { \
//        __op__(__st__* v)          { glUniformMatrix4##__gl__##v(loc, 1, GL_FALSE, v); } \
//        __op__(__vt__##4 const& v) { glUniformMatrix4##__gl__##v(loc, 1, GL_FALSE, glm::value_ptr(v)); } \
//    };

//    DECL(Mat , float , glm::mat , f)
//    DECL(DMat, double, glm::dmat, d)
//#undef DECL
}



#undef __op__
