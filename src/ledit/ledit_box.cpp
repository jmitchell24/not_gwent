//
// Created by james on 10/6/23.
//

#include "ledit_box.hpp"
#include "ledit_yaml.hpp"

using namespace ledit;

#include "id_bip0039.hpp"

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
#include <ut/check.hpp>
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
    static auto c = color::hsluv{120, 80, 80};

    c.rotate(100);
    return c.toColor();
}

static color ColWindowBg()
{
    using namespace ImGui;
    color c = GetStyleColorVec4(ImGuiCol_WindowBg);
    return c;
}

static color ColBorder()
{
    using namespace ImGui;
    color c = GetStyleColorVec4(ImGuiCol_Border);
    return c.opaque();
}

void DRAW_OVERLAY_OUTLINE(rect const &rect, color const &c, float thickness)
{
    using namespace ImGui;

    auto rr = VIRT.realRect(rect).round();
    auto dl = GetBackgroundDrawList();
    dl->AddRect(rr.min, rr.max, ToU32(c), 0, 0, thickness);
}

void DRAW_OVERLAY_BG(rect const &rect)
{
    using namespace ImGui;

    auto rr = VIRT.realRect(rect).round();
    auto dl = GetBackgroundDrawList();
    dl->AddRectFilled(rr.min, rr.max, ToU32(ColWindowBg()));
}

#include "rlgl.h"

void DRAW_OVERLAY_PADDING(rect const &outer, rect const &inner, color const &c)
{
    using namespace ImGui;

    auto o = VIRT.realRect(outer).round();
    auto i = VIRT.realRect(inner).round();

    auto dl = GetBackgroundDrawList();

    auto prev_flags = dl->Flags;

    dl->Flags = dl->Flags & ~ImDrawListFlags_AntiAliasedFill;
    dl->AddQuadFilled(o.tl(), o.tr(), i.tr(), i.tl(), ToU32(c));
    dl->AddQuadFilled(o.tr(), o.br(), i.br(), i.tr(), ToU32(c));
    dl->AddQuadFilled(o.br(), o.bl(), i.bl(), i.br(), ToU32(c));
    dl->AddQuadFilled(o.bl(), o.tl(), i.tl(), i.bl(), ToU32(c));

    dl->Flags = prev_flags;
}

//cstrview nextName()
//{
//    static size_t counter=0;
//    return bip0039::wordlistRuntimeShuffle()[ (counter++) % 2048 ].str();
//}

//
// Box -> Implementation
//

box_ptr Box::root_box = createRoot();
box_ptr Box::selected_box;

Box::TreeTableOptions Box::tree_table_options;

Box::Box(box_ptr p)
        : parent{std::move(p)}, color{nextColor()}, name{}
{}

box_ptr Box::create(box_ptr const &parent)
{
    assert(parent);
    return box_ptr{new Box(parent)};
}

box_ptr Box::ptr()
{
    return shared_from_this();
}

box_ptr Box::tryGetBox(vec2 const &mp)
{
    for (auto &&it: child_boxes)
    {
        if (auto box = it->tryGetBox(mp))
            return box;
    }

    auto r = VIRT.realRect(bounds_content);

    if (r.contains(mp))
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

void Box::drawProperties()
{
    using namespace ImGui;

    assert(ptr() == selected_box);

    Text("Selection");

    if (parent)
    {
        if (Button("to parent"))
        {
            selected_box = parent;
        }

        SameLine();

        if (ButtonConfirm("delete"))
        {
            selected_box = parent;
            parent->setRowAction({RowAction::REMOVE, ptr()});
        }

        SameLine();

        if (Button("--"))
        {
            parent->setRowAction({RowAction::MOVE_DEC, ptr()});
        }

        SameLine();

        if (Button("++"))
        {
            parent->setRowAction({RowAction::MOVE_INC, ptr()});
        }
    } else
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
    LabelText("bounds_content", "%s", to_string(bounds_content.psize()).c_str());
    ButtonEnabled(" ", false);
    SameLine();
    LabelText("bounds_parent", "%s", to_string(bounds_parent.psize()).c_str());
    ButtonEnabled(" ", false);
    SameLine();
    LabelText("bounds_pad", "%s", to_string(bounds_pad.psize()).c_str());

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
        if (SmallButton((*it)->getLbl().c_str())) selected_box = (*it);
        PopButtonColor();

        for (++it; it != ps.rend(); ++it)
        {
            SameLine();
            TextUnformatted("/");
            SameLine();
            PushButtonColor((*it)->color);
            if (SmallButton((*it)->getLbl().c_str())) selected_box = (*it);
            PopButtonColor();
        }
    }


    if (ButtonDefault("name", !name.empty()))
    { name.clear(); }

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


        bool show_row_select = tree_table_options.show_row_select;
        tree_table_options.show_row_select = false;
        for (auto &&it: child_boxes)
            it->drawTreeTableRow(true);
        tree_table_options.show_row_select = show_row_select;

        applyRowAction();

        EndTable();
    }
}

void Box::drawTreeTableRow()
{
    if (child_boxes.empty())
    {
        drawTreeTableRow(true);
    } else if (drawTreeTableRow(false))
    {
        for (auto &&it: child_boxes)
            it->drawTreeTableRow();
        applyRowAction();
        ImGui::TreePop();
    }
}

bool Box::drawTreeTableRow(bool is_leaf)
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
            selected_box = ptr();
        }

        PopStyleColor();
    }

    //
    // Controls
    //

    if (TableNextColumn())
    {
        if (tree_table_options.show_row_select)
        {
            if (SmallButtonActivated("o", ptr() == selected_box))
                selected_box = ptr();
            SameLine();
        }

        if (tree_table_options.show_row_add)
        {
            if (SmallButton("+"))
                child_boxes.push_back(Box::create(ptr()));
            SameLine();
        }

        if (parent && tree_table_options.show_row_delete)
        {
            if (SmallButtonConfirm("x"))
                parent->setRowAction({RowAction::REMOVE, ptr()});
            SameLine();
        }

        if (parent && tree_table_options.show_row_move)
        {
            if (SmallButton("--"))
                parent->setRowAction({RowAction::MOVE_DEC, ptr()});
            SameLine();

            if (SmallButton("++"))
                parent->setRowAction({RowAction::MOVE_INC, ptr()});
            SameLine();
        }

        if (tree_table_options.show_row_rename)
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

        if (tree_table_options.show_row_weight)
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

        if (tree_table_options.show_row_type)
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


void Box::drawOverlay()
{
    root_box->layout(rect{});

    DRAW_OVERLAY_BG(root_box->bounds_content);

    if (selected_box)
    {
        selected_box->drawOverlaySelectedBelow();
        root_box->drawOverlayOutlines();
        selected_box->drawOverlaySelectedAbove();
    } else
    {
        root_box->drawOverlayOutlines();
    }
}

void Box::drawOverlaySelectedBelow()
{

    if (sizer.hasPad())
    {
        DRAW_OVERLAY_PADDING(bounds_pad, bounds_content, color.withNormalA(.25f));
    }

}

void Box::drawOverlaySelectedAbove()
{
    DRAW_OVERLAY_OUTLINE(bounds_content, color, 3.0f);
}

void Box::drawOverlayOutlines()
{
    using namespace ImGui;

    if (!isSelected())
    {
        DRAW_OVERLAY_OUTLINE(bounds_content, ColBorder(), 1.0f);
    }

    for (auto &&it: child_boxes)
    {
        it->drawOverlayOutlines();
    }
}

void Box::reset()
{
    name = "";
    weight = 1;
    sizer.reset();
    child_boxes.clear();
}

void Box::layout(rect const &parent)
{
    auto b = sizer.getBox(parent);
    bounds_parent = b.parent;
    bounds_pad = b.pad;
    bounds_content = b.content;

    if (child_boxes.empty())
        return;

    switch (type)
    {
        case BOX_VBOX:
            layoutVbox(bounds_pad);
            break;
        case BOX_HBOX:
            layoutHbox(bounds_pad);
            break;
        case BOX_SBOX:
            layoutSbox(bounds_pad);
            break;
        default:
            nopath_case(BoxType);
    }
}

void Box::layoutVbox(rect const &b)
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

void Box::layoutHbox(rect const &b)
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

bool Box::loadYaml(cstrparam filename)
{
    if (filename.empty())
        return false;

    if (FILE *file = fopen(filename, "r"))
    {
        try
        {
            auto text = gulp::file_to_string(file);

            box_ptr b = createRoot();
            fromYaml(YAML::Load(text), b);
            root_box = b;
            selected_box = root_box;
            return true;
        }
        catch (YAML::Exception const &ex)
        {
            fprintf(stderr, "%s:%d:%d:\n    ", filename.c_str(), ex.mark.line, ex.mark.column);
            fprintf(stderr, "YAML parse error: %s\n", ex.msg.c_str());
        }

        fclose(file);
    } else
    {
        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
    }
    return false;
}

bool Box::saveYaml(cstrparam filename)
{
    if (filename.empty())
        return false;

    if (FILE *file = fopen(filename, "w"))
    {
        YAML::Emitter emitter;

        emitYaml(emitter, root_box);

        fwrite(emitter.c_str(), sizeof(char), emitter.size(), file);
        fclose(file);
        return true;
    } else
    {
        fprintf(stderr, "error opening layout file (%s)", filename.c_str());
    }
    return false;
}

void Box::drawWindowSelectedBox()
{
    using namespace ImGui;

    if (!GetIO().WantCaptureMouse)
    {
        if (IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (auto box = root_box->tryGetBox(GetMousePos()))
                selected_box = box;
            else
                selected_box = nullptr;
        }

        if (IsMouseClicked(ImGuiMouseButton_Middle))
        {
            if (auto parent = ledit::Box::selected_box->parent)
                selected_box = parent;
        }

        if (IsMouseClicked(ImGuiMouseButton_Right))
        {
            if (auto box = root_box->tryGetBox(GetMousePos()))
            {
                if (auto parent = box->parent)
                    selected_box = parent;
            }
        }
    }

    if (Begin("Box Properties###selected_box"))
    {
        if (selected_box)
        {
            selected_box->drawProperties();
        } else
        {
            PushItemDisabled();
            Text("No Box Selected");
            PopItemDisabled();
        }
    }
    End();
}

void Box::drawWindowBoxHierarchy()
{
    using namespace ImGui;

    if (Begin("Box Hierarchy###box_hierarchy"))
    {
        auto &opts = tree_table_options;

        Text("Row Options");

        Columns(3, "row-columns", false);
        Checkbox("add", &opts.show_row_add);
        NextColumn();
        Checkbox("delete", &opts.show_row_delete);
        NextColumn();
        Checkbox("move", &opts.show_row_move);
        NextColumn();
        Checkbox("rename", &opts.show_row_rename);
        NextColumn();
        Checkbox("weight", &opts.show_row_weight);
        NextColumn();
        Checkbox("type", &opts.show_row_type);
        Columns();


        Separator();
        Text("Hierarchy");
        ImGuiTableFlags table_flags =
                ImGuiTableFlags_BordersV |
                ImGuiTableFlags_BordersOuterH |
                ImGuiTableFlags_Resizable |
                ImGuiTableFlags_RowBg |
                ImGuiTableFlags_NoBordersInBody;

        if (BeginTable("grids", 2, table_flags))
        {
            TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
            TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed);
            TableHeadersRow();

            root_box->drawTreeTableRow();

            EndTable();
        }
    }
    End();
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

                case RowAction::MOVE_DEC:
                    if (it != b)
                        it->swap(*(it - 1));
                    break;

                case RowAction::MOVE_INC:
                    if (auto down = it + 1; down != e)
                        it->swap(*down);
                    break;
            }
            return;
        }
    }
}

void Box::setRowAction(RowAction const &ra)
{
    assert(!m_row_action);
    m_row_action = ra;
}

box_ptr Box::createRoot()
{
    auto box = box_ptr{new Box({})};
    box->sizer.setDimUnits({1280, 720});
    box->sizer.setPosUnits({0, 0});
    return box;
}

bool Box::isSelected()
{

    return ptr() == selected_box;
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
