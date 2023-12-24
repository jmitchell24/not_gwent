//
// Created by james on 9/23/23.
//

#pragma once


#include "scene.hpp"

//
// game
//
#include "game/game2.hpp"

//
// ImGui
//
#include "rlImGui/imgui/imgui_mods.hpp"

class SceneGameBoard2Test : public Scene
{
public:
    ut::cstrview name() const override
    {
        return "GameBoard2 Test";
    }

    void layout() override
    {
        m_gb.layout(Context::VIEW_RECT);
    }

    void update(update_param u) override
    {
        m_gb.update(u);
        game::TANK.update(u);
    }

    bool draw_board=true;
    bool draw_overlay=true;

    void draw() override
    {
        m_gb.drawUnderBoardCards();

        if (draw_board)
            game::TANK.draw(game::CARD_LAYER_BOARD);

        m_gb.drawAboveBoardCards();
        m_gb.drawUnderOverlayCards();

        if (draw_overlay)
            game::TANK.draw(game::CARD_LAYER_OVERLAY);

        m_gb.drawAboveOverlayCards();
    }

    void drawDebug() override
    {
        using namespace ImGui;

        if (ButtonSelected("Layer Board", draw_board)) { draw_board = !draw_board; }
        if (ButtonSelected("Layer Overlay", draw_overlay)) { draw_overlay = !draw_overlay; }

        m_gb.drawDebug();
        game::TANK.drawDebug();
    }

    void drawDebugMenu() override
    {
        m_gb.drawDebugMenu();
    }

private:
    game::GameBoard2 m_gb;
};