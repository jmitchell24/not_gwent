//
// Created by james on 10/1/23.
//

#pragma once

#include "gfx/gfx_draw.hpp"

#include "rlImGui/imgui/imgui_mods.hpp"

#define flt_        float
#define vec2_       ut::vec2f const&
#define rect_       ut::rectf const&
#define color_      ut::color const&
#define text_       ut::cstrparam

namespace gfx
{

    class Prototype
    {
    public:
        static Prototype& instance()
        {
            static Prototype x;
            return x;
        }

        inline void drawRaisedBezel(rect_ outer, rect_ inner, flt_ f1, flt_ f2, color_ c)
        {
            auto _c = c.toHSLUV();
            auto co = _c.withL(_c.l * f2).toColor();
            auto ci = _c.withL(_c.l * f1).toColor();

            Quad trap_t { { outer.tl(), co }, { inner.tl(), ci }, { inner.tr(), ci }, { outer.tr(), co } };
            Quad trap_l { { outer.tl(), co }, { outer.bl(), co }, { inner.bl(), ci }, { inner.tl(), ci } };
            Quad trap_b { { inner.bl(), ci }, { outer.bl(), co }, { outer.br(), co }, { inner.br(), ci } };
            Quad trap_r { { inner.tr(), ci }, { inner.br(), ci }, { outer.br(), co }, { outer.tr(), co } };

            Quad q_inner{ { inner.tl(), c  }, { inner.bl(), ci }, { inner.br(), ci }, { inner.tr(), c  } };

            drawQuad(trap_t);
            drawQuad(trap_l);
            drawQuad(trap_b);
            drawQuad(trap_r);
            drawQuad(q_inner);
        }

        inline void drawLoweredBezel(rect_ outer, rect_ inner, flt_ f1, flt_ f2, color_ c)
        {
            auto _c = c.toHSLUV();
            auto co = _c.withL(_c.l * f1).toColor();
            auto ci = _c.withL(_c.l * f2).toColor();

            Quad trap_t { { outer.tl(), co }, { inner.tl(), ci }, { inner.tr(), ci }, { outer.tr(), co } };
            Quad trap_l { { outer.tl(), co }, { outer.bl(), co }, { inner.bl(), ci }, { inner.tl(), ci } };
            Quad trap_b { { inner.bl(), ci }, { outer.bl(), co }, { outer.br(), co }, { inner.br(), ci } };
            Quad trap_r { { inner.tr(), ci }, { inner.br(), ci }, { outer.br(), co }, { outer.tr(), co } };

            Quad q_inner{ { inner.tl(), ci }, { inner.bl(), co }, { inner.br(), co }, { inner.tr(), ci } };


            drawQuad(trap_t);
            drawQuad(trap_l);
            drawQuad(trap_b);
            drawQuad(trap_r);
            drawQuad(q_inner);
        }

        inline void drawRaisedBox(rect_ r, color_ c)
        {

            gfx::drawShadow(r, {2,2}, 5);

            auto outer = r;
            auto inner = outer.deflated(m_dimens.panel_bezel);

            //drawRaisedBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, c);

            outer = inner.deflated(m_dimens.panel_frame);
            inner = outer.deflated(m_dimens.panel_bezel);
            //drawLoweredBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, c);
        }

        inline void drawPanel1(rect_ r)
        {
            drawRaisedBox(r, m_colors.wood);
        }

        inline void drawAvatar(rect_ r, Texture2D t)
        {
            gfx::drawShadow(r, {2,2}, 5);

            auto outer = r;
            auto inner = outer.deflated(m_dimens.panel_bezel/2);

            //drawRaisedBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, m_colors.iron);

            outer = inner.deflated(m_dimens.panel_frame/2);
            inner = outer.deflated(m_dimens.panel_bezel/2);
            //drawLoweredBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, m_colors.iron);

            drawTexture(t, inner, ut::colors::white);
        }

        void drawWoodFrame();
        void drawIronPanel();

        struct Board
        {
            Quad q_outer;
            Quad q_inner1;
            Quad q_inner2;
            Quad q_inner3;
        };

        Board makeBoard(rect_ outer, rect_ inner, color_ c)
        {
            //assert(outer.contains(inner));

            auto s0 = m_dimens.shade0(c);
            auto s1 = m_dimens.shade1(c);
            auto s2 = m_dimens.shade2(c);
            auto s3 = m_dimens.shade3(c);

            auto ex = (inner.top - outer.top) / 10;

            return
            {
                .q_outer  {outer, s3},
                .q_inner1 {inner, s2},
                .q_inner2 {inner.inflated(ex), s1},
                .q_inner3 {inner, s0},
            };

        }

        Board makeWoodBoard(rect_ outer, rect_ inner)
        {
            return makeBoard(outer, inner, m_colors.wood);
        }

        Board makeIronBoard(rect_ outer, rect_ inner)
        {
            return makeBoard(outer, inner, m_colors.iron);
        }


        void drawBoard(Board const& b)
        {
            drawFrame(b.q_outer, b.q_inner1);
            drawQuad(b.q_inner2);
            drawQuad(b.q_inner3);
        }

        void drawWoodBoard(rect_ outer, rect_ inner)
        {
            //assert(outer.contains(inner));
            drawBoard(makeBoard(outer, inner, m_colors.wood));
        }

        void drawIronBoard(rect_ outer, rect_ inner)
        {
            //assert(outer.contains(inner));
            drawBoard(makeBoard(outer, inner, m_colors.iron));
        }

        void drawNerfBoard(rect_ outer, rect_ inner)
        {
            //assert(outer.contains(inner));
            drawBoard(makeBoard(outer, inner, m_colors.nerf));
        }

        void drawBuffBoard(rect_ outer, rect_ inner)
        {
            //assert(outer.contains(inner));
            drawBoard(makeBoard(outer, inner, m_colors.buff));
        }

        struct Frame { Quad q1,q2,q3,q4,q5,q6,q7,q8; };

        Frame makeFrame(rect_ outer, rect_ inner, color_ c)
        {
            //assert(outer.contains(inner));

            auto s0 = m_dimens.shade0(c);
            auto s1 = m_dimens.shade1(c);
            auto s2 = m_dimens.shade2(c);
            auto s3 = m_dimens.shade3(c);

            auto g = (inner.top - outer.top);

            auto r1 = outer;
            auto r2 = outer.deflated(g * .4f);
            auto r3 = outer.deflated(g * .6f);
            auto r4 = inner;
            auto r5 = inner.deflated(inner.width() * .1f, inner.height() * .1f);

            return
            {
                .q1  = Quad{r1, s3},
                .q2  = Quad{r2, s2},
                .q3  = Quad{r2, s1},
                .q4  = Quad{r3, s1},
                .q5  = Quad{r3, s2},
                .q6  = Quad{r4, s3},
                .q7  = Quad{r4, ut::colors::black.withNormalA(.70f)},
                .q8  = Quad{r5, ut::colors::black.withNormalA(.01f)}
            };
        }

        void drawIronFlat(rect_ r)
        {
            gfx::drawQuad(Quad(r, darken(m_colors.iron)));
        }

        ut::color darken(color_ c)
        {
            auto hsluv = c.toHSLUV();
            return hsluv.withL(hsluv.l*.85f).toColor();
        }

        void drawWoodFlat(rect_ r)
        {

            gfx::drawQuad(Quad(r, darken(m_colors.wood)));
        }

        void drawIronFrame(rect_ outer, rect_ inner)
        {
            Frame f = makeFrame(outer, inner, m_colors.iron);

            gfx::drawFrame(f.q1, f.q2);
            gfx::drawFrame(f.q3, f.q4);
            gfx::drawFrame(f.q5, f.q6);
            //gfx::drawFrame(f.q7, f.q8);
        }


        void drawShadow(rect_ outer)
        {
            gfx::drawShadow(outer, {0,0}, 6);
        }



        void drawIronSeparator();
        void drawWoodSeparator();

        void drawDebug()
        {
            auto tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
            if (ImGui::TreeNodeEx("Proto", tree_flags))
            {
                ImGui::SliderFloat("Panel Bezel", &m_dimens.panel_bezel, 1.0f, 20.0f);
                ImGui::SliderFloat("Panel Frame", &m_dimens.panel_frame, 1.0f, 20.0f);
                ImGui::SliderFloat4("Shade Factors", m_dimens.shade_factors, 0.0f, 1.0f);

                drawDebugColor(m_colors.wood, "Wood");
                drawDebugColor(m_colors.iron, "Iron");
                drawDebugColor(m_colors.buff, "Buff");
                drawDebugColor(m_colors.nerf, "Nerf");

                ImGui::TreePop();
            }
        }

        void drawDebugColor(ut::color& c, text_ lbl)
        {
            if (auto v = c.toNormal().toVec4(); ImGui::ColorEdit4(lbl, v.pack))
            {
                c = ut::color(v);
            }
        }


    private:
        struct Colors
        {
            ut::color wood = ut::color(102, 71, 39);
            ut::color iron = ut::color::parseRGBA("#8D9099FF");

            ut::color nerf = ut::color::parseRGBA("#B1301CFF");
            ut::color buff = ut::color::parseRGBA("#307EA6FF");
        } m_colors;

        struct Dimens
        {
            float panel_frame   = 7.5f;
            float panel_bezel   = 7.5f;
            float shade_factors[4]{ 1.00f, 0.80f, 0.74f, 0.37f, };

            inline ut::color shade0(color_ c) const { auto _c = c.toHSLUV(); return _c.withL(_c.l*shade_factors[0]).toColor(); }
            inline ut::color shade1(color_ c) const { auto _c = c.toHSLUV(); return _c.withL(_c.l*shade_factors[1]).toColor(); }
            inline ut::color shade2(color_ c) const { auto _c = c.toHSLUV(); return _c.withL(_c.l*shade_factors[2]).toColor(); }
            inline ut::color shade3(color_ c) const { auto _c = c.toHSLUV(); return _c.withL(_c.l*shade_factors[3]).toColor(); }
        } m_dimens;
    };

    [[nodiscard]] static Prototype& PROTO = Prototype::instance();




}

#undef flt_
#undef vec2_
#undef rect_
#undef color_
#undef text_