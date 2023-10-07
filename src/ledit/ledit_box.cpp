//
// Created by james on 10/6/23.
//

#include "ledit_box.hpp"
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

using namespace ut;
//
// std
//
using namespace std;

//
// Box -> Implementation
//

color Box::nextColor()
{
    static size_t counter = 0;
    auto hue = float(counter++) * 100.0f + 120.0f;
    hue = fmodf(hue, 360.0f);
    return color::hsluv{hue, 80.0f, 80.0f, 1.0f}.toColor();
}

box_ptr Box::selected_box;

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

cstrview getBoxName(BoxType type)
{
    switch (type)
    {
        case BOX_VBOX: return "vbox";
        case BOX_HBOX: return "hbox";
        case BOX_SBOX: return "sbox";

        default:nopath_case(BoxType);
    }
    return "";
}

void Box::drawPopup()
{
    assert(ptr() == selected_box);

    using namespace ImGui;
    sizer.draw();


    Separator();

    Button("R"); SameLine();
    if (BeginCombo("Type", getBoxName(type)))
    {
        if (Selectable("vbox", type == BOX_VBOX)) { type = BOX_VBOX; }
        if (Selectable("hbox", type == BOX_HBOX)) { type = BOX_HBOX; }
        if (Selectable("sbox", type == BOX_SBOX)) { type = BOX_SBOX; }
        EndCombo();
    }

    Separator();

    static std::array<char, 20> name_buffer;

    ::strncpy(name_buffer.data(), name.c_str(), name_buffer.size());
    Button("R"); SameLine();
    if (InputText("name", name_buffer.data(), name_buffer.size()))
    {
        name = name_buffer.data();
    }

    Button("R"); SameLine();
    if (int w = weight; InputInt("weight", &w, 1))
    {
        if (w > 0) weight = w;
    }

    Button("R"); SameLine();
    if (Button("add"))
    {
        child_boxes.push_back(Box::create(ptr()));
    }
    
    if (parent)
    {
        if (Button("up"))
        {
            selected_box = parent;
        }
        else if (Button("delete"))
        {
            selected_box = parent;
            parent->rowAction({RowAction::REMOVE, ptr()});
        }
    }
    else
    {
        PushItemDisabled();
        Button("up");
        Button("delete");
        PopItemDisabled();
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
        doRowAction();

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
        doRowAction();
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
            parent->rowAction({RowAction::REMOVE, ptr()});
        }

        SameLine();

        if (SmallButton("up"))
        {
            parent->rowAction({RowAction::MOVE_UP, ptr()});
        }

        SameLine();

        if (SmallButton("down"))
        {
            parent->rowAction({RowAction::MOVE_DOWN, ptr()});
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

box_ptr Box::tryGetBox(vec2 const& mp)
{
    auto r = gfx::VIRT.realRect(bounds_outer);

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
