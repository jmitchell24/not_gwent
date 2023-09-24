//
// Created by james on 9/23/23.
//

#pragma once
#include "scene.hpp"

#include "game/card.hpp"
#include "ng/ng_card_data.hpp"

#include "rlImGui/imgui/imgui_extra.hpp"


class SceneCardTest : public Scene
{
public:
    char const* name() const override { return "Card Test"; }

    game::Card card;

    void layout(ut::rect const& b) override
    {
        card.layout = game::layout::CardLayout::fromRect(b.anchorCCtoCC(b.size() * .8));
        card.assets = game::Card::Assets::fromNgCard(ng::cards::neutral_avallach);
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
    }
};