//
// Created by james on 10/27/23.
//

#include "scene/scene_stage.hpp"

//
// gfx
//
#include "gfx/gfx_prototype.hpp"
using namespace gfx;

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

void Stage::load  () { m_selected->load(); }
void Stage::unload() { m_selected->unload(); }
void Stage::layout() { for (auto&& it : m_scenes) it->layout(); }
void Stage::update(update_param u) { m_selected->update(u); }
void Stage::draw() { m_selected->draw(); }

void Stage::drawDebug()
{
    using namespace ImGui;

    if (m_show_debug_scene)
    {
        if (Begin("Scene"))
        {
            BeginChild(m_selected->name(), {0, 0}, true);
            m_selected->drawDebug();
            EndChild();
        }
        End();
    }

    if (m_show_debug_graphics)
    {
        Begin("Graphics");
        DRECT.drawDebug();
        PROTO.drawDebug();
        End();
    }


}

void Stage::drawDebugMenu()
{
    using namespace ImGui;

    if (BeginMenu("File"))
    {
        if (MenuItem("Exit"))
        {
            should_exit = true;
        }
        EndMenu();
    }

    if (BeginMenu("Edit"))
    {
        EndMenu();
    }

    if (BeginMenu("View"))
    {
        if (MenuItem("Scene Debug", nullptr, m_show_debug_scene))
            m_show_debug_scene = !m_show_debug_scene;

        if (MenuItem("Graphics Debug", nullptr, m_show_debug_graphics))
            m_show_debug_graphics = !m_show_debug_graphics;
        EndMenu();
    }

    if (BeginMenu("Scene"))
    {
        for (auto&& it : m_scenes)
        {
            if (MenuItem(it->name(), nullptr, m_selected == it))
            {
                check(m_selected != nullptr, "selected ptr was null");
                m_selected->unload();
                m_selected = it;
                m_selected->load();
            }

        }


        EndMenu();
    }

    if (BeginMenu("Help"))
    {
        EndMenu();
    }

    Separator();

    Value("FPS", GetFPS());
    Value("Frame MS", GetFrameTime());
}