//
// Created by james on 8/29/23.
//

#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

#define _flt        float
#define _vec2       ut::vec2f const&
#define _rect       ut::rectf const&
#define _color      ut::color const&
#define _text       ut::cstrparam



#define CASE(x_) \
void ::gfx::drawText##x_(_rect r, _flt h, _text s, _color c)  \
{ \
    assert(h > 0); \
    auto r_text = r.anchor##x_(float(MeasureText(s.c_str(), int(h))),h); \
    DrawText(s.c_str(), r_text.min.x, r_text.min.y, h, torl(c)); \
} \
void ::gfx::drawText##x_(_rect r, _text s, _color c)  \
{ ::gfx::drawText##x_(r, r.height(), s, c); } \
void ::gfx::drawText##x_(Font font, _rect r, _flt h, _text s, _color c)  \
{ \
    assert(h > 0); \
    auto r_text = r.anchor##x_(tout(MeasureTextEx(font, s.c_str(), h, 2))); \
    DrawTextEx(font, s.c_str(), Vector2{r_text.min.x, r_text.min.y}, h, 2, torl(c)); \
} \
void ::gfx::drawText##x_(Font font, _rect r, _text s, _color c)  \
{ ::gfx::drawText##x_(font, r, r.height(), s, c); }

UT_ENUM_RECT_ALIGNMENTS
#undef CASE




void ::gfx::drawCircle(_vec2 center, _flt radius, _color color)
{
    return DrawCircle((int)center.x, (int)center.y, radius, torl(color));
}

void ::gfx::drawRect(_rect rect, _color color)
{
    DrawRectangleRec(torl(rect), torl(color));
}

void ::gfx::drawRectOutline(_rect rect, _flt thickness, _color color)
{
    DrawRectangleLinesEx(torl(rect), thickness, torl(color));
}

#undef _flt
#undef _vec2
#undef _rect
#undef _color
#undef _text