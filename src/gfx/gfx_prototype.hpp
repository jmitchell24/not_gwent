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

            drawQuad(trap_t);
            drawQuad(trap_l);
            drawQuad(trap_b);
            drawQuad(trap_r);
            drawRect(inner, c);
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

            drawQuad(trap_t);
            drawQuad(trap_l);
            drawQuad(trap_b);
            drawQuad(trap_r);
            drawRect(inner, co);
        }

        inline void drawRaisedBox(rect_ r, color_ c)
        {

            gfx::drawShadow(r, {2,2}, 5);

            auto outer = r;
            auto inner = outer.shrunk(m_dimens.panel_bezel);

            drawRaisedBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, c);

            outer = inner.shrunk(m_dimens.panel_frame);
            inner = outer.shrunk(m_dimens.panel_bezel);
            drawLoweredBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, c);
        }

        inline void drawPanel1(rect_ r)
        {
            drawRaisedBox(r, m_colors.wood);
        }

        inline void drawAvatar(rect_ r, Texture2D t)
        {
            gfx::drawShadow(r, {2,2}, 5);

            auto outer = r;
            auto inner = outer.shrunk(m_dimens.panel_bezel/2);

            drawRaisedBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, m_colors.iron);

            outer = inner.shrunk(m_dimens.panel_frame/2);
            inner = outer.shrunk(m_dimens.panel_bezel/2);
            drawLoweredBezel(outer, inner, m_dimens.factor_dark, m_dimens.factor_darker, m_colors.iron);

            drawTexture(t, inner, ut::colors::white);
        }

        void drawDebug()
        {
            auto tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
            if (ImGui::TreeNodeEx("Proto", tree_flags))
            {
                ImGui::SliderFloat("Panel Bezel", &m_dimens.panel_bezel, 1.0f, 20.0f);
                ImGui::SliderFloat("Panel Frame", &m_dimens.panel_frame, 1.0f, 20.0f);
                ImGui::SliderFloat("Light Factor", &m_dimens.factor_dark, 0.0f, 1.0f);
                ImGui::SliderFloat("Dark Factor", &m_dimens.factor_darker, 0.0f, 1.0f);

                if (auto v = m_colors.wood.toNormal().toVec4(); ImGui::ColorEdit4("Wood", v.pack))
                {
                    m_colors.wood = ut::color(v);
                }

                if (auto v = m_colors.iron.toNormal().toVec4(); ImGui::ColorEdit4("Iron", v.pack))
                {
                    m_colors.iron = ut::color(v);
                }

                ImGui::TreePop();
            }
        }


    private:
        struct Colors
        {
            ut::color wood = ut::color(102, 71, 39);
            ut::color iron = ut::color::parseRGBA("#8D9099FF");
        } m_colors;

        struct Dimens
        {
            float panel_frame   = 7.5f;
            float panel_bezel   = 7.5f;
            float factor_dark   = .85f;
            float factor_darker = .55f;

        } m_dimens;
    };

    [[nodiscard]] static Prototype& PROTO = Prototype::instance();




}

#undef flt_
#undef vec2_
#undef rect_
#undef color_
#undef text_