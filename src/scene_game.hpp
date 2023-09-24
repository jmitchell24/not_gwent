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
    char const* name() const override { return "GameBoard2 Test"; }

    void layout(ut::rect const& b) override
    {
        m_gb.layout(b);
    }

    void update(float dt) override
    {
        m_gb.update(dt);
        game::TANK.update(dt);
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