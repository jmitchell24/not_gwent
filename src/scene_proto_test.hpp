//
// Created by james on 10/1/23.
//


#pragma once

#include "scene.hpp"
#include "gfx/gfx_prototype.hpp"


class SceneProtoTest : public Scene
{
public:
    char const* name() const override { return "Proto Test"; }



    ut::rect bounds;

    void layout(ut::rect const& b) override
    {
        bounds = b;
    }

    void update(float dt) override
    {

    }

    void draw() override
    {
        gfx::PROTO.drawPanel1(bounds.anchorCCtoCC(100,100));
    }

    void drawDebug() override
    {

    }
};