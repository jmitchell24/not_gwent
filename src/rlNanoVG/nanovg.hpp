//
// Created by james on 10/3/23.
//

#include "external/glad.h"

extern "C" {
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"
}

#include <ut/math.hpp>


namespace nvg
{
    class Context
    {
    public:
//        Context()
//            : m_context{nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG)}
//        {
//
//        }
//
//        ~Context()
//        {
//            nvgDeleteGL3(m_context);
//        }

        static Context& instance()
        {
            static Context x;
            return x;
        }

    private:
        NVGcontext* m_context;
    };
}

nvg::Context& NVG = nvg::Context::instance();