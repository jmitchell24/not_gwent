//
// Created by james on 9/23/23.
//

#pragma once

#include "context.hpp"
#include "conv.hpp"

#include "scene/scene.hpp"

#include "ledit/ledit_editor.hpp"
#include "game/card.hpp"
#include "gfx/gfx_draw.hpp"
#include "ng/ng_card_data.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"


class SceneCardTest : public Scene
{
public:
    ut::cstrview name() const override { return "Card Test"; }

    game::Card card;

    ledit::BoxEditor editor{"card.yaml"};

    ut::rect r_strength;
    ut::rect r_ability;
    ut::rect r_row;

    ut::rect r_bounds;

    void layout() override
    {
        auto b = Context::VIEW_RECT;
        card.layout = game::layout::CardLayout::fromRect(b.anchorCCtoCC(b.size() * .8));
        card.ng     = ng::cards::neutral_avallach;
        card.assets = game::Card::Assets::fromNgCard(card.ng);

        editor.setRoot(card.layout.getRect());
    }

    void update(update_param u) override
    {
        card.update(u.frame_time);
    }

    void drawCircle(ut::rect const& bounds, ut::color const& col)
    {
        auto c = bounds.center();
        auto w = bounds.width();
        auto h = bounds.height();
        auto r = (w>h ? h : w) / 2;

        DrawCircle(c.x, c.y, r, torl(col));
    }

    void draw() override
    {
        card.draw();


        drawCircle(r_strength, ut::colors::red);
        drawCircle(r_ability, ut::colors::green);
        drawCircle(r_row, ut::colors::blue);

        gfx::drawTexturePro(card.assets.row, r_row, ut::colors::white);
    }

    void drawDebug() override
    {
        ImGui::LabelText("layout size", "%.1f, %.1f",
                         card.layout.m_w, card.layout.m_h);

        ImGui::LabelText("layout pos", "%.1f, %.1f",
                         card.layout.m_x, card.layout.m_y);

        ImGui::LabelText("layout elevation", "%.1f",
                         card.layout.m_z);


        editor.view_transform = gfx::DRECT.view_transform;

        if (editor.draw())
        {
            if (auto box = editor.getRect("strength"))
            {
                r_strength = *box;
            }

            if (auto box = editor.getRect("ability"))
            {
                r_ability = *box;
            }

            if (auto box = editor.getRect("row"))
            {
                r_row = *box;
            }
        }
    }
};