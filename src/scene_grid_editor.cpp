//
// Created by james on 10/3/23.
//

#include "scene_grid_editor.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
#include <ut/check.hpp>
#include <ut/time.hpp>
using namespace ut;

//
// yaml
//
#include <yaml-cpp/yaml.h>

//
// std
//
#include <filesystem>
using namespace std;


void SceneLayoutEditor::layout(rect const &b)
{
    bounds = b;
}

void SceneLayoutEditor::update(float dt)
{

}

void SceneLayoutEditor::draw()
{


}

void SceneLayoutEditor::drawDebug()
{
    using namespace ImGui;
    auto r = gfx::VIRT.virtViewport();

    ledit::Box::drawOverlay();

    ledit::Box::drawWindowSelectedBox();

    ledit::Box::drawWindowBoxHierarchy();

    ledit::Box::drawWindowYamlOutput();

    ledit::Box::drawWindowCPPOutput();

    if (Begin("Box Document###box_files"))
    {
        if (ButtonConfirm("Reset Root"))
        {
            ledit::Box::root_box = ledit::Box::createRoot();
            ledit::Box::selected_box = nullptr;
        }

        if (ButtonDefault("active_file", !m_active_filename.empty()))
            m_active_filename.clear();

        BeginGroup();

        if (m_active_filename.empty())
        {
            if (BeginCombo("filename", "..."))
            {
                for (auto&& it : m_filenames)
                {
                    if (Selectable(it.c_str(), it == m_active_filename))
                    {
                        if (ledit::Box::loadYaml(it))
                            m_active_filename = it;
                    }

                    if (IsItemHovered())
                        SetTooltip("%s", it.c_str());
                }
                EndCombo();
            }

            if (IsItemClicked())
            {
                refreshFiles();
            }
        }
        else
        {
            LabelText("filename", "%s", m_active_filename.c_str());
        }

        EndGroup();

        if (m_active_filename.empty())
        {
            if (Button("Create File"))
            {
                m_active_filename = PRINTER("data/layout/%s.yaml", ledit::Box::root_box->name.c_str());
                ledit::Box::saveYaml(m_active_filename);
            }
        }
        else
        {
            if (Button("Overwrite File"))
            {
                ledit::Box::saveYaml(m_active_filename);
            }
        }
    }
    End();
}

void SceneLayoutEditor::refreshFiles()
{
    m_filenames.clear();
    for (auto&& it : filesystem::directory_iterator("data/layout"))
    {
        m_filenames.push_back(it.path().string());
    }
}

#if 0

//
// sol/lua binding
//

namespace sol
{
    //https://blog.rubenwardy.com/2020/07/22/sol3-vector-extension/

    inline vec2 sol_lua_get(sol::types<vec2>, lua_State* L, int index, sol::stack::record& tracking)
    {
        int absoluteIndex = lua_absindex(L, index);

        auto table = sol::stack::get<sol::table>(L, absoluteIndex);
        vec2::scalar_type x = table["x"];
        vec2::scalar_type y = table["y"];

        tracking.use(1);

        return { x, y };
    }

    inline int sol_lua_push(sol::types<vec2>, lua_State* L, vec2 const& pos)
    {
        lua_createtable(L, 0, 3);

        lua_getglobal(L, "Vec2");
        lua_setmetatable(L, -2);

        sol::stack_table v(L);
        v["x"] = pos.x;
        v["y"] = pos.y;

        return 1;
    }

    template<>
    struct lua_type_of<vec2> : std::integral_constant<sol::type, sol::type::table> {};


    template <typename Handler>
    inline bool sol_lua_check(sol::types<vec2>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking)
    {
        int absoluteIndex = lua_absindex(L, index);
        if (!stack::check<sol::table>(L, absoluteIndex, handler)) {
            tracking.use(1);
            return false;
        }

        sol::stack::get_field(L, "x", absoluteIndex);
        bool x = sol::stack::check<float>(L, -1);

        sol::stack::get_field(L, "y", absoluteIndex);
        bool y = sol::stack::check<float>(L, -1);

        sol::stack::pop_n(L, 3);

        tracking.use(1);
        return x && y;
    }

}

#endif