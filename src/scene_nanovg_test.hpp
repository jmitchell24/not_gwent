//
// Created by james on 10/11/23.
//

#pragma once

#include <regex>
#include "scene.hpp"
#include "gfx/gfx_virt2d.hpp"
#include "game/asset/assets.hpp"
#include "rlImGui/imgui/imgui.h"

#include "rlNanoVG/nanovg.hpp"

class SceneNanoVGTest : public Scene
{
public:
    char const* name() const override { return "NanoVG Test"; }

    float ratio1=1.5;
    float ratio2=1;

    ut::rect bounds;

    NSVGimage* svg_image;

    void layout(ut::rect const& b) override
    {
        bounds = b;

        char const* filename = "data/svg/tiger.svg";

        svg_image = nsvgParseFromFile(filename, "px", 96.0f);

        if (!svg_image)
        {
            TraceLog(LOG_ERROR, ut::PRINTER("Failed to load file: %s", filename).c_str());
        }
        else
        {
            TraceLog(LOG_ERROR, "Tiger is loaded...");
        }

    }

    void update(float dt) override
    {

    }

    Texture2D texture = game::svg::tiger();

    void draw() override
    {
        auto r = bounds.anchorCCtoCC(ratio1 * 300, 300);

        gfx::VIRT.drawTexturePro(texture, bounds, ut::colors::white);

        DRECT2(r1, r);
        DRECT2(r2, r.fitAspect(ratio2));

//        NVG.beginDrawing();
//        gfx::VIRT.setNanoVGViewport(NVG.m_context);
//
//        check_null(svg_image);
//        nsvg_Draw(svg_image);
//
//        NVG.endDrawing();


    }

    void drawDebug() override
    {
        using namespace ImGui;

        SliderFloat("ratio1", &ratio1, 0.5, 2.0);
        SliderFloat("ratio2", &ratio2, 0.5, 2.0);

/*	printf("Tris: draws:%d  fill:%d  stroke:%d  text:%d  TOT:%d\n",
		ctx->drawCallCount, ctx->fillTriCount, ctx->strokeTriCount, ctx->textTriCount,
		ctx->fillTriCount+ctx->strokeTriCount+ctx->textTriCount);*/

        auto vg = NVG.m_context;
        NVGstats stats;
        nvgGetStats(vg, &stats);

        Value("draws", stats.drawCallCount);
        Value("fill", stats.fillTriCount);
        Value("text", stats.textTriCount);
        Value("stroke", stats.strokeTriCount);
        Value("total", stats.fillTriCount+stats.textTriCount+stats.strokeTriCount);


    }

    inline NVGcolor SVGCOLOR(unsigned int c, float r, float g, float b, float a)
    {
        return nvgRGBA((c & 0xFF) * r, ((c >> 8) & 0xFF) * g, ((c >> 16) & 0xFF) * b, ((c >> 24) & 0xFF) * a);
    }


    NVGpaint createLinearGradient(NSVGgradient* gradient, float r, float g, float b, float a)
    {
        auto vg = NVG.m_context;

        float inverse[6];
        float sx, sy, ex, ey;

        nvgTransformInverse(inverse, gradient->xform);
        nvgTransformPoint(&sx, &sy, inverse, 0, 0);
        nvgTransformPoint(&ex, &ey, inverse, 0, 1);

        return nvgLinearGradient(vg, sx, sy, ex, ey,
                                 SVGCOLOR(gradient->stops[0].color, r, g, b, a),
                                 SVGCOLOR(gradient->stops[gradient->nstops - 1].color, r, g, b, a));
    }

    NVGpaint createRadialGradient(NSVGgradient* gradient, float r, float g, float b, float a)
    {
        auto vg = NVG.m_context;

        float inverse[6];
        float cx, cy, r1, r2, inr, outr;

        nvgTransformInverse(inverse, gradient->xform);
        nvgTransformPoint(&cx, &cy, inverse, 0, 0);
        nvgTransformPoint(&r1, &r2, inverse, 0, 1);
        outr = r2 - cy;
        if (gradient->nstops == 3)
            inr = gradient->stops[1].offset * outr;
        else
            inr = 0;

        NVGpaint paint = nvgRadialGradient(vg, cx, cy, inr, outr,
                                           SVGCOLOR(gradient->stops[0].color, r, g, b, a),
                                           SVGCOLOR(gradient->stops[gradient->nstops - 1].color, r, g, g, a));

        return  paint;
    }

    void nsvg_Draw(NSVGimage*   image,
                   char const*  shapeIdPattern      =nullptr,
                   bool         strokeWidthScaling  =false,
                   float        strokeWidthFactor   =1.0f,
                   float        buildUpFactorFrom   =0.0f,
                   float        buildUpFactorTo     =1.0f,
                   ut::color::normal const& color   =ut::colors::normal::white())
    {
        auto vg = NVG.m_context;

        // skip drawing if builtup from is greater than (equal to) builtup to
        if (buildUpFactorTo >= 0 && buildUpFactorFrom >= buildUpFactorTo)
            return;

        int join, cap;
        float i, r, g, b, a;
        float buildUpFromAlpha = 1.0, buildUpToAlpha = 0.0;
        int buildUpFromCount = -1, buildUpToCount = -1;
        std::regex pattern(shapeIdPattern != nullptr ? shapeIdPattern : "");

        // prepare build-up properties
        if (buildUpFactorFrom >= 0.0)
        {
            buildUpFromAlpha = 1.0f - std::modf(buildUpFactorFrom, &i);
            buildUpFromCount = std::floor(i) + 1;
        }

        if (buildUpFactorTo >= 0.0)
        {
            buildUpToAlpha = std::modf(buildUpFactorTo, &i);
            buildUpToCount = std::floor(i) + 1;
        }

        // custom color
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;

        // iterate shapes
        for (NSVGshape *shape = image->shapes; shape != nullptr; shape = shape->next)
        {

            // skip invisible shape
            if (!(shape->flags & NSVG_FLAGS_VISIBLE))
                continue;

//            // skip shape with not matching ID pattern
//            if (shapeIdPattern != nullptr &&
//                !std::regex_match(shape->id, pattern)) continue;
//
//            // decrease build-up counter or finish drawing if 0
//            if (--buildUpToCount == -1) break;
//
//            // skip shape under builtup from
//            if (--buildUpFromCount > 0) continue;
//
//            // set build-up alpha for the first visible shape
//            if (!buildUpFromCount)
//                a *= buildUpFromAlpha;
//            else if (buildUpFromCount == -1)
//                a = color.a;
//
//            // set build-up alpha for the last shape
//            if (!buildUpToCount)
//                a *= buildUpToAlpha;

            nvgBeginPath(vg);
            bool pathHole = false;

            // draw paths
            for (NSVGpath *path = shape->paths; path != nullptr; path = path->next)
            {

                if (pathHole)
                    nvgPathWinding(vg, NVG_HOLE);
                else
                    pathHole = true;

                nvgMoveTo(vg, path->pts[0], path->pts[1]);

                for (int i = 0; i < path->npts - 1; i += 3) {
                    float* p = &path->pts[i * 2];
                    nvgBezierTo(vg, p[2], p[3], p[4], p[5], p[6], p[7]);
                }

                if (path->closed)
                    nvgLineTo(vg, path->pts[0], path->pts[1]);
            }

            // fill
            switch (shape->fill.type)
            {
                case NSVG_PAINT_COLOR:
                    nvgFillColor(vg, SVGCOLOR(shape->fill.color, r, g, b, shape->opacity * a));
                    nvgFill(vg);
                    break;
                case NSVG_PAINT_LINEAR_GRADIENT:
                    nvgFillPaint(vg, createLinearGradient(shape->fill.gradient, r, g, b, shape->opacity * a));
                    nvgFill(vg);
                    break;
                case NSVG_PAINT_RADIAL_GRADIENT:
                    nvgFillPaint(vg, createRadialGradient(shape->fill.gradient, r, g, b, shape->opacity * a));
                    nvgFill(vg);
                    break;
            }

            // set stroke/line
            switch (shape->strokeLineJoin)
            {
                case NSVG_JOIN_ROUND: join = NVG_ROUND; break;
                case NSVG_JOIN_BEVEL: join = NVG_BEVEL; break;
                case NSVG_JOIN_MITER:
                default: join = NVG_MITER;
            }
            nvgLineJoin(vg, join);
            nvgLineCap(vg, shape->strokeLineCap); // NanoSVG has the same line cap constants values as NanoVG
            nvgStrokeWidth(vg, shape->strokeWidth * strokeWidthFactor);

            // draw line
            switch (shape->stroke.type)
            {
                case NSVG_PAINT_COLOR:
                    nvgStrokeColor(vg, SVGCOLOR(shape->stroke.color, r, g, b, shape->opacity * a));
                    if (strokeWidthScaling)
                        nvgStroke(vg);
                    else
                        nvgStrokeNoScale(vg);
                    break;
                case NSVG_PAINT_LINEAR_GRADIENT:
                    nvgStrokePaint(vg, createLinearGradient(shape->stroke.gradient, r, g, b, shape->opacity * a));
                    if (strokeWidthScaling)
                        nvgStroke(vg);
                    else
                        nvgStrokeNoScale(vg);
                    break;
                case NSVG_PAINT_RADIAL_GRADIENT:
                    nvgStrokePaint(vg, createRadialGradient(shape->stroke.gradient, r, g, b, shape->opacity * a));
                    if (strokeWidthScaling)
                        nvgStroke(vg);
                    else
                        nvgStrokeNoScale(vg);
                    break;
            }
        }
    }
};