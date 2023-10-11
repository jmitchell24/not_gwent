//
// Created by james on 10/3/23.
//

#include "scene_grid_editor.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"

#include "check.hpp"

//
// ut
//
#include <ut/time.hpp>
using namespace ut;

//
// yaml
//
#include <yaml-cpp/yaml.h>

//
// std
//
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

    ledit::Box::root_box->layout(r);
    ledit::Box::root_box->drawRect(ledit::Box::selected_box);

    if (IsMouseClicked(ImGuiMouseButton_Right))
    {
        if (auto box = ledit::Box::root_box->tryGetBox(GetMousePos()))
        {
            ledit::Box::selected_box = box;
            //OpenPopup("m_tag_popup");
        }
    }

//    if (BeginPopup("m_tag_popup"))
//    {
//        if (ledit::Box::selected_box)
//        {
//            ledit::Box::selected_box->drawPopup();
//        }
//        else
//        {
//            //CloseCurrentPopup();
//        }
//
//        EndPopup();
//    }

    //m_tag_root.normalizeWeights();

    if (Button("Save"))
    {
        ledit::Box::saveYaml("data/layout/test.yaml");
    }

    if (Button("Load"))
    {
        ledit::Box::loadYaml("data/layout/test.yaml");
    }

    if (Begin("Box Properties###selected_box"))
    {
        if (ledit::Box::selected_box)
        {
            ledit::Box::selected_box->drawProperties();
        }
        else
        {
            PushItemDisabled();
            Text("No Box Selected");
            PopItemDisabled();
        }
    }
    End();

    if (Begin("Box Hierarchy###box_hierarchy"))
    {
        auto& opts = ledit::Box::tree_table_options;

        auto w = ImGui::CalcTextSize("xxxxx").x;



        Text("Row Options");

        Columns(3, "row-columns",false);
        Checkbox("add"      , &opts.show_row_add    ); NextColumn();
        Checkbox("delete"   , &opts.show_row_delete ); NextColumn();
        Checkbox("move"     , &opts.show_row_move   ); NextColumn();
        Checkbox("rename"   , &opts.show_row_rename ); NextColumn();
        Checkbox("weight"   , &opts.show_row_weight ); NextColumn();
        Checkbox("type"     , &opts.show_row_type   );
        Columns();


        Separator();
        Text("Hierarchy");
        ImGuiTableFlags table_flags =
                ImGuiTableFlags_BordersV        |
                ImGuiTableFlags_BordersOuterH   |
                ImGuiTableFlags_Resizable       |
                ImGuiTableFlags_RowBg           |
                ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("grids", 2, table_flags))
        {
            ImGui::TableSetupColumn("Label" , ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ledit::Box::root_box->drawTreeTableRow();

            ImGui::EndTable();
        }
    }
    End();
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