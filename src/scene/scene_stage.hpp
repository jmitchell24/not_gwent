//
// Created by james on 10/27/23.
//

#pragma once

#include "scene/scene.hpp"

#include <initializer_list>

class Stage
{
public:
    using scenelist_type = std::vector<Scene*>;

    bool should_exit = false;

    Stage(std::initializer_list<Scene*> const& scenes);

    void load();
    void unload();
    void layout();
    void update(update_param u);
    void draw();
    void drawDebug();
    void drawDebugMenu();

private:
    scenelist_type  m_scenes;
    Scene*          m_selected;
    bool            m_show_debug_scene=true;
    bool            m_show_debug_graphics=false;
};