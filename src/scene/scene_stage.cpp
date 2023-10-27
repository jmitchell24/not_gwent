//
// Created by james on 10/27/23.
//

#include "scene/scene_stage.hpp"

//
// raylib
//
#include <raylib.h>

//
// imgui
//
#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
#include <ut/check.hpp>
using namespace ut;

//
// std
//
#include <cassert>
using namespace std;

Stage::Stage(std::initializer_list<Scene*> const& scenes)
    : m_scenes{scenes}, m_selected{m_scenes.front()}
{ assert(!m_scenes.empty()); }

void Stage::load  () { for (auto&& it : m_scenes) it->load(); }
void Stage::unload() { for (auto&& it : m_scenes) it->unload(); }
void Stage::layout() { for (auto&& it : m_scenes) it->layout(); }
void Stage::update(update_param u) { m_selected->update(u); }
void Stage::draw() { m_selected->draw(); }

void Stage::drawDebug()
{
    using namespace ImGui;

    Begin("Stage");

    Value("FPS", GetFPS());
    Value("Frame MS", GetFrameTime());


    End();
    Begin("Scene");

    if (BeginCombo("Scene", m_selected->name()))
    {
        for (auto&& it : m_scenes)
        {
            if (ImGui::Selectable(it->name(), m_selected == it))
            {
                m_selected = it;
            }
        }
        EndCombo();
    }

    BeginChild(m_selected->name(), {0,0}, true);
    m_selected->drawDebug();
    EndChild();

    End();
}