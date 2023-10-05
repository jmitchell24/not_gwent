//
// Created by james on 10/4/23.
//

#pragma once

#include "scene.hpp"
#include "gfx/gfx_virt2d.hpp"
#include "rlImGui/imgui/imgui.h"


class SceneDemoWindow : public Scene
{
public:
    char const* name() const override { return "Demo Window"; }

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

    }

    void drawDebug() override
    {
        ImGui::ShowDemoWindow();
    }
};