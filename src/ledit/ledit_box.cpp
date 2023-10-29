//
// Created by james on 10/6/23.
//

#include "ledit_box.hpp"
#include "ledit_yaml.hpp"
using namespace ledit;

//
// imgui
//
#include "rlImGui/imgui/imgui_mods.hpp"
#include "rlImGui/ed/TextEditor.h"

//
// ut
//
#include <ut/check.hpp>
#include <ut/algo.hpp>

using namespace ut;
//
// std
//
using namespace std;


#include "rlgl.h"

//
// Helper Functions
//

color Box::nextColor()
{
    static auto c = color::hsluv{120, 80, 80};
    c.rotate(100);
    return c.toColor();
}

void drawOverlayOutline(rect const &rect, color const &c, float thickness)
{
    using namespace ImGui;

    auto rr = rect.round();
    auto dl = GetBackgroundDrawList();
    auto prev_flags = dl->Flags;
    dl->Flags = dl->Flags & ~ImDrawListFlags_AntiAliasedFill;
    //dl->AddRect(rr.min, rr.max, ToU32(colors::black.withNormalA(.8)), 0, 0, thickness*5);
    dl->AddRect(rr.min, rr.max, ToU32(c), 0, 0, thickness);
    dl->Flags = prev_flags;
}

void drawOverlayPadding(rect const &outer, rect const &inner, color const &c)
{
    using namespace ImGui;

    auto o = outer.round();
    auto i = inner.round();

    auto dl = GetBackgroundDrawList();
    auto prev_flags = dl->Flags;
    dl->Flags = dl->Flags & ~ImDrawListFlags_AntiAliasedFill;
    dl->AddQuadFilled(o.tl(), o.tr(), i.tr(), i.tl(), ToU32(c));
    dl->AddQuadFilled(o.tr(), o.br(), i.br(), i.tr(), ToU32(c));
    dl->AddQuadFilled(o.br(), o.bl(), i.bl(), i.br(), ToU32(c));
    dl->AddQuadFilled(o.bl(), o.tl(), i.tl(), i.bl(), ToU32(c));

    dl->Flags = prev_flags;
}



//
// Box -> Implementation
//

Box::Box(box_ptr p)
    : parent{std::move(p)}, color{nextColor()}, name{}
{}

box_ptr Box::create(box_ptr const& parent)
{
    check_null(parent);
    return box_ptr{new Box(parent)};
}

box_ptr Box::ptr()
{
    return shared_from_this();
}

box_cptr Box::ptr() const
{
    return shared_from_this();
}

box_ptr Box::deepCopy() const
{
    nopath_impl;
}

box_ptr Box::tryGetBox(vec2 const& mp)
{
    for (auto&& it: child_boxes)
    {
        if (auto box = it->tryGetBox(mp))
            return box;
    }

    if (auto r = bounds_outer; r.contains(mp))
        return ptr();
    return nullptr;
}

string Box::getLbl()
{
    if (name.empty())
    {
        switch (type)
        {
            case BOX_VBOX:
                return PRINTER("[vbox %zu]", child_boxes.size()).str();
            case BOX_HBOX:
                return PRINTER("[hbox %zu]", child_boxes.size()).str();
            case BOX_SBOX:
                return PRINTER("[sbox %zu]", child_boxes.size()).str();
            default:
                nopath_case(BoxType);
        }
    }

    return name;
}

void Box::drawProperties(BoxVisitor& v)
{
    using namespace ImGui;

    assert(ptr() == v.selected_box);

    Text("Selection");

    if (parent)
    {
        if (Button("to parent"))
        {
            v.selected_box = parent;
        }

        SameLine();

        if (ButtonConfirm("delete"))
        {
            v.selected_box = parent;
            v.clearBox(ptr());
            parent->setChildAction({ChildAction::REMOVE, ptr()});
        }

        SameLine();

        if (Button("--"))
        {
            parent->setChildAction({ChildAction::MOVE_DEC, ptr()});
        }

        SameLine();

        if (Button("++"))
        {
            parent->setChildAction({ChildAction::MOVE_INC, ptr()});
        }
    }
    else
    {
        PushItemDisabled();
        Button("parent");
        SameLine();
        Button("delete");
        PopItemDisabled();
    }

    Separator();

    ButtonEnabled(" ", false);
    SameLine();
    LabelText("bounds_content", "%s", to_string(bounds_outer.psize()).c_str());
    ButtonEnabled(" ", false);
    SameLine();
    LabelText("bounds_pad", "%s", to_string(bounds_inner.psize()).c_str());

    {
        box_ptr b = ptr();
        vector<box_ptr> ps;

        do
        {
            ps.push_back(b);
            b = b->parent;
        } while (b);


        ButtonEnabled(" ", false);
        SameLine();


        auto it = ps.rbegin();
        PushButtonColor((*it)->color);
        if (SmallButton((*it)->getLbl().c_str())) v.selected_box = (*it);
        PopButtonColor();

        for (++it; it != ps.rend(); ++it)
        {
            SameLine();
            TextUnformatted("/");
            SameLine();
            PushButtonColor((*it)->color);
            if (SmallButton((*it)->getLbl().c_str())) v.selected_box = (*it);
            PopButtonColor();
        }
    }


    if (ButtonDefault("name", !name.empty()))
    { v.clearBox(ptr()); }

    if (BeginCombo("name###name_get", name.c_str()))
    {
        for (auto&& it : v.boxmap())
        {
            auto key = it.first;
            if (Selectable(key.c_str(), name == key))
            {
                name = key;
                v.setBox(ptr());
            }
        }

        EndCombo();
    }

    Separator();

    Text("Size and Position");

    sizer.drawProperties();

    Separator();

    Text("Container");

    if (ButtonDefault("weight", weight != 1))
    { weight = 1; }
    if (int w = weight; InputInt("weight", &w, 1))
    {
        if (w > 0) weight = w;
    }

    if (ButtonDefault("type", type != BOX_SBOX))
    { type = BOX_SBOX; }
    if (BeginCombo("Type", box_to_string(type)))
    {
        EXPAND_BOXTYPE(CASE_SELECTABLE)
        EndCombo();
    }

    if (ButtonDefault("child_boxes", !child_boxes.empty()))
    { child_boxes.clear(); }

    if (Button("add"))
    {
        child_boxes.push_back(Box::create(ptr()));
    }

    ImGuiTableFlags table_flags =
            ImGuiTableFlags_BordersV |
            ImGuiTableFlags_BordersOuterH |
            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_NoBordersInBody;

    if (BeginTable("gridsxxx", 2, table_flags))
    {
        TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
        TableHeadersRow();


        bool show_row_select = v.edit_opts.show_row_select;
        v.edit_opts.show_row_select = false;
        for (auto &&it: child_boxes)
            it->drawTreeTableRow(v, true);
        v.edit_opts.show_row_select = show_row_select;

        EndTable();
    }
}

void Box::drawTreeTableRow(BoxVisitor& v)
{
    if (child_boxes.empty())
    {
        drawTreeTableRow(v, true);
    }
    else if (drawTreeTableRow(v, false))
    {
        for (auto &&it: child_boxes)
            it->drawTreeTableRow(v);
        ImGui::TreePop();
    }
}

bool Box::drawTreeTableRow(BoxVisitor& v, bool is_leaf)
{
    using namespace ImGui;

    bool is_open = false;

    PushID(this);

    TableNextRow();

    //
    // TreeNode
    //
    if (TableNextColumn())
    {
        PushStyleColor(ImGuiCol_Text, color);

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

        is_open = TreeNodeEx(getLbl().c_str(), flags);
        highlighted = IsItemHovered();

        if (is_leaf && IsItemClicked(ImGuiMouseButton_Left))
        {
            v.selected_box = ptr();
        }

        PopStyleColor();
    }

    //
    // Controls
    //

    if (TableNextColumn())
    {
        if (v.edit_opts.show_row_select)
        {
            if (SmallButtonActivated("o", ptr() == v.selected_box))
                v.selected_box = ptr();
            SameLine();
        }

        if (v.edit_opts.show_row_add)
        {
            if (SmallButton("+"))
                child_boxes.push_back(Box::create(ptr()));
            SameLine();
        }

        if (parent && v.edit_opts.show_row_delete)
        {
            if (SmallButtonConfirm("x"))
                parent->setChildAction({ChildAction::REMOVE, ptr()});
            SameLine();
        }

        if (parent && v.edit_opts.show_row_move)
        {
            if (SmallButton("--"))
                parent->setChildAction({ChildAction::MOVE_DEC, ptr()});
            SameLine();

            if (SmallButton("++"))
                parent->setChildAction({ChildAction::MOVE_INC, ptr()});
            SameLine();
        }

        if (v.edit_opts.show_row_rename)
        {
            if (SmallButton("n"))
                OpenPopup("show-row-rename");

            if (BeginPopup("show-row-rename"))
            {
                std::array<char, 20> static name_buffer;
                ::strncpy(name_buffer.data(), name.c_str(), name_buffer.size());
                if (InputText("name", name_buffer.data(), name_buffer.size()))
                    name = name_buffer.data();
                EndPopup();
            }
            SameLine();
        }

        if (v.edit_opts.show_row_weight)
        {
            Text("%d", weight);
            SameLine();

            if (SmallButton("+"))
                weight++;
            SameLine();

            if (SmallButtonEnabled("-", weight > 1))
                weight--;
            SameLine();
        }

        if (v.edit_opts.show_row_type)
        {
            if (SmallButton("t"))
                OpenPopup("show-row-type");

            if (BeginPopup("show-row-type"))
            {
                EXPAND_BOXTYPE(CASE_SELECTABLE)
                EndPopup();
            }
            SameLine();
        }
    }

    PopID();

    return is_open;
}

void Box::drawOverlaySelectedBelow(BoxVisitor& v)
{
    using namespace ImGui;

    auto i = v.getRect(bounds_inner);
    auto o = v.getRect(bounds_outer);
    drawOverlayPadding(i, o, color.withNormalA(.25f));
}

void Box::drawOverlaySelectedAbove(BoxVisitor& v)
{
    using namespace ImGui;

    auto o = v.getRect(bounds_outer);
    drawOverlayOutline(o, color, 3.0f);
}

void Box::drawOverlayOutlines(BoxVisitor& v)
{
    using namespace ImGui;

    if (ptr() != v.selected_box)
    {
        auto o = v.getRect(bounds_outer);
        drawOverlayOutline(o, v.overlay_opts.border, 1.0f);
    }

    for (auto &&it: child_boxes)
    {
        it->drawOverlayOutlines(v);
    }
}

void Box::reset()
{
    name = "";
    weight = 1;
    sizer.reset();
    child_boxes.clear();
}

void Box::layout(rect const& p)
{
    auto rects = sizer.getRects(p);

    bounds_inner    = rects.pad;
    bounds_outer    = rects.content;

    if (child_boxes.empty())
        return;

    switch (type)
    {
        case BOX_VBOX:
            layoutVbox(bounds_inner);
            break;
        case BOX_HBOX:
            layoutHbox(bounds_inner);
            break;
        case BOX_SBOX:
            layoutSbox(bounds_inner);
            break;
        default:
            nopath_case(BoxType);
    }
}

void Box::layoutVbox(rect const& b)
{
    auto sz = child_boxes.size();
    auto h = b.height() - (inner_pad * float(sz - 1));
    auto sum = weightsSum();
    auto cl = b.min.x;
    auto cr = b.max.x;

    auto cy = b.min.y;

    for (size_t i = 0; i < sz - 1; ++i)
    {
        auto &&it = child_boxes[i];
        auto ch = h * (float(it->weight) / sum);
        auto ct = cy;
        auto cb = cy + ch;

        it->layout({cl, ct, cr, cb});

        cy = cb + inner_pad;
    }

    child_boxes.back()->layout({cl, cy, cr, b.max.y});
}

void Box::layoutHbox(rect const& b)
{
    auto sz = child_boxes.size();
    auto w = b.width() - (inner_pad * float(sz - 1));
    auto sum = weightsSum();
    auto ct = b.min.y;
    auto cb = b.max.y;

    auto cx = b.min.x;

    for (size_t i = 0; i < sz - 1; ++i)
    {
        auto &&it = child_boxes[i];
        auto cw = w * (float(it->weight) / sum);
        auto cl = cx;
        auto cr = cx + cw;

        it->layout({cl, ct, cr, cb});

        cx = cr + inner_pad;
    }

    child_boxes.back()->layout({cx, ct, b.max.x, cb});
}

void Box::layoutSbox(rect const &b)
{
    for (auto &&it: child_boxes)
    {
        it->layout(b);
    }
}

string Box::toYamlString()
{
    YAML::Emitter em;
    emitYaml(em, ptr());
    return em.c_str();
}

string Box::toCPPString()
{
    cstrparam txt = R"(
namespace layout
{
    struct Layout
    {
        ut::rect name1 = ut::rect();
        ut::rect name2 = ut::rect();
        ut::rect name3 = ut::rect();
        ut::rect name4 = ut::rect();
        ut::rect name5 = ut::rect();

        void layout(boxmap_t const& map)
        {
            name1 = map["name1"];
            name2 = map["name2"];
            name3 = map["name3"];
            name4 = map["name4"];
            name5 = map["name5"];
        }
    };

    ...

    //
    LAYOUT_MANAGER.bind(my_layout, "my_layout.yaml");

    using boxmap_t = std::unordered_set<std::string, ut::rect>;
}



    )"_sv;

    return txt.str();
}

bool Box::loadYaml(cstrparam filename)
{
    if (filename.empty())
        return false;

    if (auto file = fopen(filename, "r"))
    {
        try
        {
            auto text = gulp::file_to_string(file);
            fromYaml(YAML::Load(text), ptr());
            return true;
        }
        catch (YAML::Exception const &ex)
        {
            fprintf(stderr, "%s:%d:%d:\n    ", filename.c_str(), ex.mark.line, ex.mark.column);
            fprintf(stderr, "YAML parse error: %s\n", ex.msg.c_str());
        }

        fclose(file);
    }
    else
    {
        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
    }
    return false;
}

bool Box::saveYaml(cstrparam filename)
{
    if (filename.empty())
        return false;

    if (auto file = fopen(filename, "w"))
    {
        auto str = toYamlString();
        fwrite(str.c_str(), sizeof(char), str.size(), file);
        fclose(file);
        return true;
    }
    else
    {
        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
    }
    return false;
}





void Box::applyChildActions()
{
    if (m_child_action)
    {
        auto b  = child_boxes.begin();
        auto e  = child_boxes.end();
        auto ca = *m_child_action;

        m_child_action.reset();

        for (auto it = b; it != e; ++it)
        {
            if (*it == ca.box)
            {
                switch (ca.type)
                {
                    case ChildAction::REMOVE:
                        child_boxes.erase(it);
                        break;

                    case ChildAction::MOVE_DEC:
                        if (it != b)
                            it->swap(*(it - 1));
                        break;

                    case ChildAction::MOVE_INC:
                        if (auto down = it + 1; down != e)
                            it->swap(*down);
                        break;
                }
                return;
            }

            (*it)->applyChildActions();
        }
    }
    else
    {
        for (auto && it : child_boxes)
            it->applyChildActions();
    }

}

void Box::setChildAction(ChildAction const& child_action)
{
    assert(!m_child_action);
    m_child_action = child_action;
}

box_ptr Box::createRoot(rect const& bounds)
{
    Sizer sz;
    sz.setDimUnits(bounds.size());
    sz.setPosUnits(bounds.pos());
    return createRoot(sz);
}

box_ptr Box::createRoot(Sizer const& sizer)
{
    auto box = box_ptr{new Box({})};
    box->sizer = sizer;
    return box;
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
