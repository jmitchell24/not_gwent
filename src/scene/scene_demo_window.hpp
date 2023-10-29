//
// Created by james on 10/4/23.
//

#pragma once

#include "scene.hpp"
#include "rlImGui/imgui/imgui.h"


class SceneDemoWindow : public Scene
{
public:
    [[nodiscard]] ut::cstrview name() const override
    {
        return "Demo Window";
    }

    void drawDebug() override
    {
        ImGui::ShowDemoWindow();
    }
};