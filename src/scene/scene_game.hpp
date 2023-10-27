//
// Created by james on 9/23/23.
//

#pragma once


#include "scene.hpp"

//
// game
//
#include "game/game2.hpp"

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
        m_gb.update(u.frame_time);
        game::TANK.update(u.frame_time);
    }

    void draw() override
    {
        m_gb.drawUnderCards();
        game::TANK.draw();
        m_gb.drawAboveCards();
    }

    void drawDebug() override
    {
        m_gb.drawDebug();
        game::TANK.drawDebug();
    }

private:
    game::GameBoard2 m_gb;
};