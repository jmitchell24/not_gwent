//
// Created by james on 10/11/23.
//

#include "external/glad.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "rlNanoVG/nanovg.h"
#include "rlNanoVG/nanovg_gl.h"
#include "rlNanoVG/nanovg_gl_utils.h"

#define NANOSVG_IMPLEMENTATION
#include "rlNanoVG/nanosvg.h"

#include "rlNanoVG/nanovg.hpp"
using namespace nvg;

#include "raylib.h"

//
// ut
//
#include <ut/check.hpp>

Context& Context::instance()
{
    static Context x;
    return x;
}

void Context::load()
{
    if (auto ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG))
    {
        m_context = ctx;


        if (auto font = nvgCreateFont(m_context, "sans", "data/font/FiraSans-Regular.ttf"); font != -1)
        {
            nvgAddFallbackFontId(m_context, font, font);
        }
        else
        {
            TraceLog(LOG_FATAL, "Failed to create NanoVG fallback font");
        }


    }
    else
    {
        TraceLog(LOG_FATAL, "Failed to create NanoVG Context");
    }
}

void Context::unload()
{
    check_null(m_context);
}

void Context::beginDrawing()
{
    check_null(m_context);
    nvgBeginFrame(m_context, 1280, 720, 96);
}

void Context::endDrawing()
{
    nvgEndFrame(m_context);
}