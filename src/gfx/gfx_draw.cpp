//
// Created by james on 8/29/23.
//

#include "gfx/gfx_draw.hpp"
using namespace gfx;



//
// ut
//
#include <ut/check.hpp>
using namespace ut;

#define flt_        float
#define vec2_       ut::vec2f const&
#define rect_       ut::rectf const&
#define color_      ut::color const&
#define text_       ut::cstrparam

#define CASE(x_) \
void ::gfx::drawText##x_(rect_ r, flt_ h, text_ s, color_ c)  \
{ \
    auto r_text = r.anchor##x_(float(MeasureText(s.c_str(), int(h))),h); \
    DrawText(s.c_str(), r_text.min.x, r_text.min.y, h, torl(c)); \
} \
void ::gfx::drawText##x_(rect_ r, text_ s, color_ c)  \
{ ::gfx::drawText##x_(r, r.height(), s, c); } \
void ::gfx::drawText##x_(Font font, rect_ r, flt_ h, text_ s, color_ c)  \
{ \
    auto r_text = r.anchor##x_(tout(MeasureTextEx(font, s.c_str(), h, 2))); \
    DrawTextEx(font, s.c_str(), Vector2{r_text.min.x, r_text.min.y}, h, 2, torl(c)); \
} \
void ::gfx::drawText##x_(Font font, rect_ r, text_ s, color_ c)  \
{ ::gfx::drawText##x_(font, r, r.height(), s, c); }

UT_ENUM_RECT_ALIGNMENTS
#undef CASE

void ::gfx::drawCircle(vec2_ center, flt_ radius, color_ color)
{
    return DrawCircle((int)center.x, (int)center.y, radius, torl(color));
}

void ::gfx::drawRect(rect_ rect, color_ color)
{
    DrawRectangleRec(torl(rect), torl(color));
}

void ::gfx::drawRectOutline(rect_ rect, flt_ thickness, color_ color)
{
    DrawRectangleLinesEx(torl(rect), thickness, torl(color));
}


void ::gfx::drawTexture(Texture2D texture, rect_ rect, color_ color)
{
    Rectangle source = { 0,0,(float)texture.width,(float)texture.height };
    Vector2 origin   = { 0, 0 };
    DrawTexturePro(texture, source, torl(rect), origin, 0, torl(color));
}

void ::gfx::drawTexture(Texture2D texture, rect_ rect)
{
    drawTexture(texture, rect, ut::colors::white);
}

void ::gfx::drawTextureFit(Texture2D texture, rect_ rect, color_ color)
{
    auto sz = rect.fit((float)texture.width, (float)texture.height);
    drawTexture(texture, rect.anchorCCtoCC(sz), color);
}

void ::gfx::drawTexturePro(Texture2D texture, rect_ rect, color_ color)
{
    drawTexture(texture, rect, color);
}

void ::gfx::drawTexturePro(Texture2D texture, rect_ src, rect_ dst, vec2_ origin, float rotation)
{
    DrawTexturePro(texture, torl(src), torl(dst), torl(origin), rotation, WHITE);
}

void ::gfx::drawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4)
{
    Texture2D texShapes = { 1, 1, 1, 1, 7 };                // Texture used on shapes drawing (usually a white pixel)
    Rectangle texShapesRec = { 0.0f, 0.0f, 1.0f, 1.0f };    // Texture source rectangle used on shapes drawing


    rlSetTexture(texShapes.id);

    rlBegin(RL_QUADS);
    rlNormal3f(0.0f, 0.0f, 1.0f);

    // NOTE: Default raylib font character 95 is a white square
    rlColor4ub(col1.r, col1.g, col1.b, col1.a);
    rlTexCoord2f(texShapesRec.x/texShapes.width, texShapesRec.y/texShapes.height);
    rlVertex2f(rec.x, rec.y);

    rlColor4ub(col2.r, col2.g, col2.b, col2.a);
    rlTexCoord2f(texShapesRec.x/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
    rlVertex2f(rec.x, rec.y + rec.height);

    rlColor4ub(col3.r, col3.g, col3.b, col3.a);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, (texShapesRec.y + texShapesRec.height)/texShapes.height);
    rlVertex2f(rec.x + rec.width, rec.y + rec.height);

    rlColor4ub(col4.r, col4.g, col4.b, col4.a);
    rlTexCoord2f((texShapesRec.x + texShapesRec.width)/texShapes.width, texShapesRec.y/texShapes.height);
    rlVertex2f(rec.x + rec.width, rec.y);
    rlEnd();

    rlSetTexture(0);
}

void ::gfx::drawRectangleGradientEx(rect_ rec, color_ col1, color_ col2, color_ col3, color_ col4)
{
    drawRectangleGradientEx(torl(rec), torl(col1), torl(col2), torl(col3), torl(col4));
}

void ::gfx::drawTriangle(vec2_ v1, vec2_ v2, vec2_ v3, color_ c1, color_ c2, color_ c3)
{
    rlBegin(RL_TRIANGLES);
    rlColor4ub(c1.r, c1.g, c1.b, c1.a); rlVertex2f(v1.x, v1.y);
    rlColor4ub(c2.r, c2.g, c2.b, c2.a); rlVertex2f(v2.x, v2.y);
    rlColor4ub(c3.r, c3.g, c3.b, c3.a); rlVertex2f(v3.x, v3.y);
    rlEnd();
}

void ::gfx::drawQuad(Quad const& q)
{
    rlSetTexture(1);

    rlBegin(RL_QUADS);
    rlColor4ub(q.tl.c.r, q.tl.c.g, q.tl.c.b, q.tl.c.a); rlVertex2f(q.tl.p.x, q.tl.p.y);
    rlColor4ub(q.bl.c.r, q.bl.c.g, q.bl.c.b, q.bl.c.a); rlVertex2f(q.bl.p.x, q.bl.p.y);
    rlColor4ub(q.br.c.r, q.br.c.g, q.br.c.b, q.br.c.a); rlVertex2f(q.br.p.x, q.br.p.y);
    rlColor4ub(q.tr.c.r, q.tr.c.g, q.tr.c.b, q.tr.c.a); rlVertex2f(q.tr.p.x, q.tr.p.y);
    rlEnd();

    rlSetTexture(0);
}

void ::gfx::drawFrame(Quad const& outer, Quad const& inner)
{
    drawQuad({ outer.tl, inner.tl, inner.tr, outer.tr });
    drawQuad({ outer.tl, outer.bl, inner.bl, inner.tl });
    drawQuad({ inner.bl, outer.bl, outer.br, inner.br });
    drawQuad({ inner.tr, inner.br, outer.br, outer.tr });
}

void ::gfx::drawShadow(rect_ rect, vec2_ offset, flt_ thickness)
{
    auto r = rect.withOffset(offset);
    auto r_shadowl  = r.anchorLCtoRC(thickness, r.height());
    auto r_shadowt  = r.anchorTCtoBC(r.width(), thickness);
    auto r_shadowr  = r.anchorRCtoLC(thickness, r.height());
    auto r_shadowb  = r.anchorBCtoTC(r.width(), thickness);





    // tl, bl, br, tr
    auto blk = torl(ut::colors::black.withNormalA(0.5));
    auto clr = blk;
    clr.a = 0;

    drawRectangleGradientEx(torl(r_shadowl), clr, clr, blk, blk);
    drawRectangleGradientEx(torl(r_shadowr), blk, blk, clr, clr);
    drawRectangleGradientEx(torl(r_shadowt), clr, blk, blk, clr);
    drawRectangleGradientEx(torl(r_shadowb), blk, clr, clr, blk);


    drawTriangle(r.tr(), r_shadowr.tr(), r_shadowt.tr(), tout(blk), tout(clr), tout(clr));
    drawTriangle(r.tl(), r_shadowt.tl(), r_shadowl.tl(), tout(blk), tout(clr), tout(clr));
    drawTriangle(r.bl(), r_shadowl.bl(), r_shadowb.bl(), tout(blk), tout(clr), tout(clr));
    drawTriangle(r.br(), r_shadowb.br(), r_shadowr.br(), tout(blk), tout(clr), tout(clr));

    drawRect(r, tout(blk));

    DRECT_PUSH2(drawShadow(), r);
    DRECT1(r_shadowl);
    DRECT1(r_shadowr);
    DRECT1(r_shadowt);
    DRECT1(r_shadowb);
    DRECT_POP();
}

#undef flt_
#undef vec2_
#undef rect_
#undef color_
#undef text_