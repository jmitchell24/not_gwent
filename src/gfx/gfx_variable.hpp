#pragma once

#include "assert.hpp"
#include "external/glad.h"

#include <ut/string.hpp>

// [my enum], [opengl enum], [glsl type], [cpp type]

#define GFX_ENUM_GLSL_VARIABLES \
    CASE(FLOAT, GL_FLOAT     , float, float    ) \
    CASE(VEC2 , GL_FLOAT_VEC2, vec2 , ut::vec2f) \
    CASE(VEC3 , GL_FLOAT_VEC3, vec3 , ut::vec3f) \
    CASE(VEC4 , GL_FLOAT_VEC4, vec4 , ut::vec4f) \
    \
    CASE(INT  , GL_INT     , int  , int      ) \
    CASE(IVEC2, GL_INT_VEC2, ivec2, ut::vec2i) \
    CASE(IVEC3, GL_INT_VEC3, ivec3, ut::vec3i) \
    CASE(IVEC4, GL_INT_VEC4, ivec4, ut::vec4i) \
    \
    CASE(UINT , GL_UNSIGNED_INT     , unsigned int, unsigned int) \
    CASE(UVEC2, GL_UNSIGNED_INT_VEC2, uvec2       , ut::vec2u   ) \
    CASE(UVEC3, GL_UNSIGNED_INT_VEC3, uvec3       , ut::vec3u   ) \
    CASE(UVEC4, GL_UNSIGNED_INT_VEC4, uvec4       , ut::vec4u   ) \
    \
    CASE(MAT4, GL_FLOAT_MAT4, mat4, Matrix)

namespace gfx
{
    struct GLSLVariable
    {
        enum Type
        {
            INVALID=-1,
#define CASE(x,y,...) x=y,
GFX_ENUM_GLSL_VARIABLES
#undef CASE
        } gl_type=INVALID;

        inline ut::cstrview glTypeName() const
        {
            using namespace ut;

            switch (gl_type)
            {
#define CASE(x_, y_, z_, w_) case x_ : return #x_##_sv;
GFX_ENUM_GLSL_VARIABLES
#undef CASE
            default:assert_case(Type);
            }
            return "???"_sv;
        }
    };



}
