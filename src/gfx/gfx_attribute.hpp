#pragma once

#include "gfx_variable.hpp"

namespace gfx
{
    struct Attribute : public GLSLVariable
    {
        char const* name    = nullptr;
        GLint       loc     = -1;

        inline bool valid() const { return loc != -1; }
    };
}
