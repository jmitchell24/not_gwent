//
// Created by james on 10/3/23.
//

#include "scene_grid_editor.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"

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

static color nextColor()
{
    static size_t counter = 0;
    auto hue = float(counter++) * 100.0f + 120.0f;
    hue = fmodf(hue, 360.0f);
    return color(color::hsluv{hue, 80.0f, 80.0f, 1.0f});
}

SceneLayoutEditor::GridTag SceneLayoutEditor::m_tag_root   {};
SceneLayoutEditor::GridTag SceneLayoutEditor::m_tag_pending{};

SceneLayoutEditor::GridTag::GridTag()
        : color{nextColor()}
{

}

void SceneLayoutEditor::GridTag::drawRect()
{
    if (child_tags.empty())
    {
        ImGui::DrawDRECT("asdf", bounds, color);
    }
    else
    {
        ImGui::DrawDRECT("asdf", bounds, color.withA(0));
        for (auto&& it : child_tags)
        {
            it.drawRect();
        }
    }
}

void SceneLayoutEditor::GridTag::draw()
{
    if (child_tags.empty())
    {
        draw(true);
    }
    else if (draw(false))
    {
        for (auto &&it: child_tags)
            it.draw();
        ImGui::TreePop();
    }
}

//void SceneLayoutEditor::GridTag::drawRect(rect const &parent)
//{
//    using namespace ImGui;
//
//
//    if (child_tags.empty())
//    {
//        DrawDRECT("node", r, color, ImGuiDRECTStyle_LabelOnly);
//    }
//    else
//    {
//        for (auto&& it: child_tags)
//        {
//            it.drawRect(r);
//        }
//    }
//}

bool SceneLayoutEditor::GridTag::draw(bool is_leaf)
{
    using namespace ImGui;

    TableNextRow();
    TableNextColumn();

    PushID(this);

    int flags =
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_DefaultOpen;

    if (is_leaf)
    {
        flags =
                ImGuiTreeNodeFlags_SpanFullWidth |
                ImGuiTreeNodeFlags_Leaf |
                ImGuiTreeNodeFlags_Bullet |
                ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    PushStyleColor(ImGuiCol_Text, color);

    char const* name = "node";
    if (type == VBOX) name = "vbox";
    if (type == HBOX) name = "hbox";

    bool open = TreeNodeEx(name, flags);
    highlighted = IsItemHovered();

    PopStyleColor();

    TableNextColumn();

//    if (SmallButton("+"))
//    {
//        OpenPopup("add-child-node");
//    }
//
//    if (BeginPopup("add-child-node"))
//    {
//        InputText("Name", m_tag_pending.name_buffer.data(), 20);
//
//        if (isNameValid(m_tag_pending.name()))
//        {
//            if (Button("Add"))
//            {
//                m_tag_pending.col = nextColor();
//                child_tags.push_back(m_tag_pending);
//                CloseCurrentPopup();
//            }
//        }
//        else
//        {
//            PushItemDisabled();
//            Button("Add");
//            PopItemDisabled();
//        }
//
//        EndPopup();
//    }


//    if (IsItemHovered())
//        SetTooltip("Add Child Node");

//    SameLine();

    if (SmallButton("a"))
    {
        child_tags.emplace_back();
    }

    if (IsItemHovered())
        SetTooltip("add");

    SameLine();

    if (SmallButton("f"))
    {
        type = type == VBOX ? HBOX : VBOX;
    }

    if (IsItemHovered())
        SetTooltip("flip");

    SameLine();

    if (weight > 1)
    {
        if (SmallButton("-"))
            weight--;
    }
    else
    {
        PushItemDisabled();
        SmallButton("-");
        PopItemDisabled();
    }

    SameLine();

    if (SmallButton("+"))
    {
        weight++;
    }

    SameLine();

    Text("%d", weight);

    PopID();

    return open;
}

void SceneLayoutEditor::layout(rect const &b)
{
    bounds = b;


}

void SceneLayoutEditor::update(float dt)
{

}

void SceneLayoutEditor::draw()
{


    //
}

void SceneLayoutEditor::drawDebug()
{
    using namespace ImGui;
    auto r = psize(1280, 720).rect();

    //m_tag_root.drawRect(r);
    m_tag_root.layout(r);
    m_tag_root.drawRect();
    m_tag_root.normalizeWeights();

    ImGuiTableFlags table_flags =
            ImGuiTableFlags_BordersV |
            ImGuiTableFlags_BordersOuterH |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_NoBordersInBody;

    if (ImGui::BeginTable("grids", 2, table_flags))
    {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        m_tag_root.draw();

        ImGui::EndTable();
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