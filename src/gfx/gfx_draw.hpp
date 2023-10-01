//
// Created by james on 8/25/23.
//

#pragma once

#include <ut/math.hpp>

#include "raylib.h"
#include "rlgl.h"
#include "conv.hpp"
#include "gfx_virt2d.hpp"

#define flt_        float
#define vec2_       ut::vec2f const&
#define rect_       ut::rectf const&
#define color_      ut::color const&
#define text_       ut::cstrparam

namespace gfx
{
#define CASE(x_) \
        void drawText##x_(rect_ r, text_ s, color_ c); \
        void drawText##x_(rect_ r, flt_ h, text_ s, color_ c); \
        void drawText##x_(Font font, rect_ r, text_ s, color_  c); \
        void drawText##x_(Font font, rect_ r, flt_ h, text_ s, color_ c);
UT_ENUM_RECT_ALIGNMENTS
#undef CASE

    void drawCircle(vec2_ center, flt_ radius, color_ color);
    void drawRect(rect_ rect, color_ color);
    void drawRectOutline(rect_ rect, flt_ thickness, color_ color);

    void drawTexture(Texture2D texture, rect_ rect, color_ color);
    void drawTexture(Texture2D texture, rect_ rect);
    void drawTextureFit(Texture2D texture, rect_ rect, color_ color);



    void drawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4);
    void drawRectangleGradientEx(rect_ rec, color_ col1, color_ col2, color_ col3, color_ col4);

    void drawShadow(rect_ rect, vec2_ offset, flt_ thickness);

    struct Vertex { ut::vec2 p; ut::color c=ut::colors::white; };
    struct Quad { Vertex v1, v2, v3, v4; };
    void drawQuad(Quad const& q);



    struct Triangle { ut::vec2 v1,v2,v3; };
    void drawTriangle(vec2_ v1, vec2_ v2, vec2_ v3, color_ c1, color_ c2, color_ c3);
}

#undef flt_
#undef vec2_
#undef rect_
#undef color_
#undef text_