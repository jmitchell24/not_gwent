//
// Created by james on 10/6/23.
//

#include "ledit_box.hpp"
#include "ledit_yaml.hpp"
using namespace ledit;

#include "assert_msg.hpp"

//
// imgui
//
#include "rlImGui/imgui/imgui_mods.hpp"

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
using namespace gfx;

//
// ut
//
#include <ut/algo.hpp>
using namespace ut;
//
// std
//
using namespace std;

//
// Helper Functions
//

color Box::nextColor()
{
    static size_t counter = 0;
    auto hue = float(counter++) * 100.0f + 120.0f;
    hue = fmodf(hue, 360.0f);
    return color::hsluv{hue, 80.0f, 80.0f, 1.0f}.toColor();
}

/*

vbox:


 */



//
// Box -> Implementation
//

box_ptr Box::root_box = box_ptr { new Box({}) };
box_ptr Box::selected_box;

Box::Box(box_ptr p)
    : parent{std::move(p)}, color{nextColor()}
{}

box_ptr Box::create(box_ptr const& parent)
{
    assert(parent);
    return box_ptr { new Box(parent) };
}

box_ptr Box::ptr()
{
    return shared_from_this();
}

box_ptr Box::tryGetBox(vec2 const& mp)
{
    auto r = VIRT.realRect(bounds_outer);

    for (auto&& it : child_boxes)
    {
        if (auto box = it->tryGetBox(mp))
            return box;
    }

    if (r.contains(mp))
        return ptr();
    return nullptr;
}

string Box::getLbl()
{
    string s;

    switch (type)
    {
        case BOX_VBOX: s += PRINTER("[vbox %zu] ", child_boxes.size()); break;
        case BOX_HBOX: s += PRINTER("[hbox %zu] ", child_boxes.size()); break;
        case BOX_SBOX: s += PRINTER("[sbox %zu] ", child_boxes.size()); break;
        default:nopath_case(BoxType);
    }

    if (ptr() == selected_box)
        s += "*** ";

    if (!name.empty())
        s += PRINTER("\"%s\"", name.c_str());

    return s;
}

void Box::drawProperties()
{
    using namespace ImGui;

    assert(ptr() == selected_box);

    Text("Selection");

    if (parent)
    {
        if (Button("parent"))
        {
            selected_box = parent;
        }

        SameLine();

        if (Button("delete"))
        {
            selected_box = parent;
            parent->setRowAction({RowAction::REMOVE, ptr()});
        }

        SameLine();

        if (Button("--"))
        {
            parent->setRowAction({RowAction::MOVE_UP, ptr()});
        }

        SameLine();

        if (Button("++"))
        {
            parent->setRowAction({RowAction::MOVE_DOWN, ptr()});
        }
    }
    else
    {
        PushItemDisabled();
        Button("parent"); SameLine();
        Button("delete");
        PopItemDisabled();
    }

    Separator();

    PushItemDisabled();
    LabelText("outer", "%s", to_string(bounds_outer.psize()).c_str());
    LabelText("inner", "%s", to_string(bounds_inner.psize()).c_str());
    PopItemDisabled();

    if (Button("R##name")) { name.clear(); } SameLine();

    std::array<char, 20> static name_buffer;
    ::strncpy(name_buffer.data(), name.c_str(), name_buffer.size());
    if (InputText("name", name_buffer.data(), name_buffer.size()))
    {
        name = name_buffer.data();
    }

    Separator();

    Text("Size and Position");

    sizer.drawProperties();

    Separator();

    Text("Container");

    if (Button("R##type")) { type = BOX_SBOX; } SameLine();
    if (BeginCombo("Type", box_to_string(type)))
    {
        auto& x = type;
        EXPAND_BOXTYPE(CASE_SELECTABLE)
        EndCombo();
    }

    if (Button("R##weight")) { weight = 1; } SameLine();
    if (int w = weight; InputInt("weight", &w, 1))
    {
        if (w > 0) weight = w;
    }

    if (Button("R##child_boxes")) { child_boxes.clear(); } SameLine();

    if (Button("add"))
    {
        child_boxes.push_back(Box::create(ptr()));
    }

    ImGuiTableFlags table_flags =
            ImGuiTableFlags_BordersV        |
            ImGuiTableFlags_BordersOuterH   |
            ImGuiTableFlags_Resizable       |
            ImGuiTableFlags_RowBg           |
            ImGuiTableFlags_NoBordersInBody;

    if (ImGui::BeginTable("gridsxxx", 2, table_flags))
    {
        ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (auto&& it : child_boxes)
            it->drawTreeTableRow(true);
        applyRowAction();

        ImGui::EndTable();
    }
}

void Box::drawTreeTableRow()
{
    if (child_boxes.empty())
    {
        drawTreeTableRow(true);
    }
    else if (drawTreeTableRow(false))
    {
        for (auto&& it: child_boxes)
            it->drawTreeTableRow();
        applyRowAction();
        ImGui::TreePop();
    }
}

bool Box::drawTreeTableRow(bool is_leaf)
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

    bool open = TreeNodeEx(getLbl().c_str(), flags);
    highlighted = IsItemHovered();

    PopStyleColor();

    TableNextColumn();

    if (ptr() == selected_box)
    {
        PushStyleColor(ImGuiCol_Button, ut::colors::goldenrod);
        SmallButton("o");
        PopStyleColor();
    }
    else if (SmallButton("o"))
    {
        selected_box = ptr();
    }

    if (parent)
    {
        SameLine();

        if (SmallButton("x"))
        {
            parent->setRowAction({RowAction::REMOVE, ptr()});
        }

        SameLine();

        if (SmallButton("up"))
        {
            parent->setRowAction({RowAction::MOVE_UP, ptr()});
        }

        SameLine();

        if (SmallButton("down"))
        {
            parent->setRowAction({RowAction::MOVE_DOWN, ptr()});
        }

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
        Text("%d", weight);


        SameLine();

        if (SmallButton("+"))
        {
            weight++;
        }
    }


    PopID();

    return open;
}

void Box::drawRect(box_ptr box)
{
    if (box == ptr())
        box = nullptr;

    auto c = !box ? color : ut::colors::black ;

    if (child_boxes.empty())
    {
        ImGui::DrawBackgroundDRECT(name.c_str(), VIRT.realRect(bounds_inner), c, ImGuiDRECTStyle_LabelOnly);
    }
    else
    {
        for (auto&& it : child_boxes)
        {
            it->drawRect(box);
        }
    }

    ImGui::DrawBackgroundDRECT(name.c_str(), VIRT.realRect(bounds_outer), c.withA(0));
}

void Box::layout(rect const& b)
{
    bounds_outer = b;
    bounds_inner = sizer(bounds_outer);

    if (child_boxes.empty())
        return;

    switch (type)
    {
        case BOX_VBOX: layoutVbox(bounds_inner); break;
        case BOX_HBOX: layoutHbox(bounds_inner); break;
        case BOX_SBOX: layoutSbox(bounds_inner); break;
        default:nopath_case(BoxType);
    }
}

void Box::layoutVbox(rect const& b)
{
    auto sz  = child_boxes.size();
    auto h   = b.height() - ( inner_pad * float( sz-1 ) );
    auto sum = weightsSum();
    auto cl  = b.min.x;
    auto cr  = b.max.x;

    auto cy  = b.min.y;

    for (size_t i = 0; i < sz-1; ++i)
    {
        auto&& it   = child_boxes[i];
        auto   ch   = h * ( float(it->weight) / sum );
        auto   ct = cy;
        auto   cb = cy+ch;

        it->layout( { cl, ct, cr, cb } );

        cy = cb+inner_pad;
    }

    child_boxes.back()->layout( { cl, cy, cr, b.max.y } );
}

void Box::layoutHbox(rect const& b)
{
    auto sz  = child_boxes.size();
    auto w   = b.width() - ( inner_pad * float( sz-1 ) );
    auto sum = weightsSum();
    auto ct  = b.min.y;
    auto cb  = b.max.y;

    auto cx  = b.min.x;

    for (size_t i = 0; i < sz-1; ++i)
    {
        auto&& it   = child_boxes[i];
        auto   cw   = w * ( float(it->weight) / sum );
        auto   cl = cx;
        auto   cr = cx+cw;

        it->layout( { cl, ct, cr, cb } );

        cx = cr+inner_pad;
    }

    child_boxes.back()->layout( { cx, ct, b.max.x, cb } );
}

void Box::layoutSbox(rect const& b)
{
    for (auto&& it: child_boxes)
    {
        it->layout(b);
    }
}

void Box::loadYaml(char const* filename)
{
    if (!filename)
        return;

    if (FILE* file = fopen(filename, "r"))
    {
        auto text = gulp::file_to_string(file);

        fclose(file);
    }
}

void Box::saveYaml(char const* filename)
{
    if (!filename)
        return;

    if (FILE* file = fopen(filename, "w"))
    {
        YAML::Emitter emitter;

        emitYaml(emitter, root_box);

        fwrite(emitter.c_str(), sizeof(char), emitter.size(), file);
        fclose(file);
    }
}

void Box::applyRowAction()
{
    if (!m_row_action)
        return;

    auto ra = *m_row_action;
    m_row_action.reset();

    auto b = child_boxes.begin();
    auto e = child_boxes.end();
    for (auto it = b; it != e; ++it)
    {
        if (*it == ra.box)
        {
            switch (ra.type)
            {
                case RowAction::REMOVE:
                    child_boxes.erase(it);
                    break;

                case RowAction::MOVE_UP:
                    if (it != b)
                        it->swap(*(it-1));
                    break;

                case RowAction::MOVE_DOWN:
                    if (auto down = it+1; down != e)
                        it->swap(*down);
                    break;
            }
            return;
        }
    }
}

void Box::setRowAction(RowAction const& ra)
{
    assert(!m_row_action);
    m_row_action = ra;
}

#if 0
void normalizeWeights()
{
    if (child_tags.size() < 2)
        return;

    auto gcd = weightsGCD();

    if (gcd > 1)
    {
        for (auto&& it : child_tags)
        {
            it.weight /= gcd;
        }
    }

    for (auto&& it : child_tags)
        it.normalizeWeights();
}
#endif
