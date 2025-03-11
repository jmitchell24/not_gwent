//
// Created by james on 3/7/24.
//

#pragma once

#include "context.hpp"
#include "conv.hpp"

#include "scene/scene.hpp"

#include "game/card_tank.hpp"


class SceneTankTest : public Scene
{
public:
    ut::cstrview name() const override { return "Tank Test"; }

    void load() override
    {
        game::TANK.clear();
    }

    void unload() override
    {
        game::TANK.clear();
    }

    void layout() override
    {

    }

    void update(update_param u) override
    {
        game::TANK.update(u);
    }

    void draw() override
    {
        game::TANK.draw(game::CARD_LAYER_BOARD);
        game::TANK.draw(game::CARD_LAYER_OVERLAY);
    }

    void drawDebug() override
    {

    }
};