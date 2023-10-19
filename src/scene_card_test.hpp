//
// Created by james on 9/23/23.
//

#pragma once

#include "ledit/ledit_editor.hpp"

#include "scene.hpp"

#include "game/card.hpp"
#include "ng/ng_card_data.hpp"

#include "rlImGui/imgui/imgui.h"


class SceneCardTest : public Scene
{
public:
    char const* name() const override { return "Card Test"; }

    game::Card card;

    ledit::BoxEditor editor{"card.yaml"};

    void layout(ut::rect const& b) override
    {
        card.layout = game::layout::CardLayout::fromRect(b.anchorCCtoCC(b.size() * .8));
        card.ng     = ng::cards::neutral_avallach;
        card.assets = game::Card::Assets::fromNgCard(card.ng);
    }

    void update(float dt) override
    {
        card.update(dt);
    }

    void draw() override
    {
        card.draw();
    }

    void drawDebug() override
    {
        ImGui::LabelText("layout size", "%.1f, %.1f",
                         card.layout.m_w,
                         card.layout.m_h);

        ImGui::LabelText("layout pos", "%.1f, %.1f",
                         card.layout.m_x,
                         card.layout.m_y);

        ImGui::LabelText("layout elevation", "%.1f",
                         card.layout.m_z);

        if (editor.begin())
        {
            // update card layout
        }
        editor.end();
    }
};