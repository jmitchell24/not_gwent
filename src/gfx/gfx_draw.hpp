//
// Created by james on 8/25/23.
//

#pragma once

#include <ut/math.hpp>

#include "raylib.h"
#include "rlgl.h"
#include "game/conv.hpp"
#include "gfx_virt2d.hpp"

#define _flt        float
#define _vec2       ut::vec2f const&
#define _rect       ut::rectf const&
#define _color      ut::color const&

namespace gfx
{
    inline void drawCircle(_vec2 center, _flt radius, _color color)
    {
        return DrawCircle((int)center.x, (int)center.y, radius, torl(color));
    }

    inline void drawRect(_rect rect, _color color)
    {
        DrawRectangleRec(torl(rect), torl(color));
    }

    inline void drawRectOutline(_rect rect, _flt thickness, _color color)
    {
        DrawRectangleLinesEx(torl(rect), thickness, torl(color));
    }

    //void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)



    inline void drawTexture(Texture2D texture, _rect rect, _color color)
    {
        Rectangle source    = { 0,0,(float)texture.width,(float)texture.height };
        Vector2 origin      = {0,0};
        //Color color         = WHITE;
        DrawTexturePro(texture, source, torl(rect), origin, 0, torl(color));
    }

    inline void drawTexture(Texture2D texture, _rect rect)
    {
        drawTexture(texture, rect, ut::colors::white);
    }

    inline void drawTextureFit(Texture2D texture, _rect rect, _color color)
    {
        auto sz = rect.fit((float)texture.width, texture.height);
        drawTexture(texture, rect.anchorCCtoCC(sz), color);
    }

    inline void drawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4)
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

    inline void drawTriangle(_vec2 v1, _vec2 v2, _vec2 v3, _color c1, _color c2, _color c3)
    {
        rlBegin(RL_TRIANGLES);
        rlColor4ub(c1.r, c1.g, c1.b, c1.a); rlVertex2f(v1.x, v1.y);
        rlColor4ub(c2.r, c2.g, c2.b, c2.a); rlVertex2f(v2.x, v2.y);
        rlColor4ub(c3.r, c3.g, c3.b, c3.a); rlVertex2f(v3.x, v3.y);
        rlEnd();
    }

    inline void drawShadow(_rect rect, _vec2 offset, _flt thickness)
    {
        auto r = rect.withOffset(offset);
        auto r_shadowl  = r.anchorLCtoRC(thickness, r.height());
        auto r_shadowt  = r.anchorTCtoBC(r.width(), thickness);
        auto r_shadowr  = r.anchorRCtoLC(thickness, r.height());
        auto r_shadowb  = r.anchorBCtoTC(r.width(), thickness);

        VIRT_DEBUG(r_shadowl);
        VIRT_DEBUG(r_shadowr);
        VIRT_DEBUG(r_shadowt);
        VIRT_DEBUG(r_shadowb);

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
    }








}

#undef _flt
#undef _vec2
#undef _rect
#undef _color