//
// Created by james on 9/23/23.
//

#pragma once

#include "scene.hpp"
#include "gfx/gfx_virt2d.hpp"
#include "rlImGui/imgui/imgui.h"


class SceneMathTest : public Scene
{
public:
    ut::cstrview name() const override { return "Math Test"; }

    float ratio1=1.5;
    float ratio2=1;

    ut::rect bounds = Context::VIEW_RECT;

    void draw() override
    {
        auto r = bounds.anchorCCtoCC(ratio1 * 300, 300);

        DRECT2(r1, r);
        DRECT2(r2, r.fitAspect(ratio2));
    }

    void drawDebug() override
    {
        ImGui::SliderFloat("ratio1", &ratio1, 0.5, 2.0);
        ImGui::SliderFloat("ratio2", &ratio2, 0.5, 2.0);
    }
};