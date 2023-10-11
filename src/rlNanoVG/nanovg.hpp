//
// Created by james on 10/3/23.
//

#pragma once

#include "rlNanoVG/nanosvg.h"
#include "rlNanoVG/nanovg.h"

#include <ut/math.hpp>


namespace nvg
{
    class Context
    {
    public:
        static Context& instance();

        void load();
        void unload();

        void beginDrawing();
        void endDrawing();

    //private:
        NVGcontext* m_context= nullptr;
        int m_fallback_font=0;
    };
}

static nvg::Context& NVG = nvg::Context::instance();