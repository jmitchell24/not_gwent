//
// Created by james on 10/6/23.
//

#include "ledit_box.hpp"
#include "ledit_yaml.hpp"
#include "ledit_options.hpp"
using namespace ledit;

//
// imgui
//
#include "rlImGui/imgui/imgui_mods.hpp"
#include "rlImGui/extras/lucide_icons.hpp"

//
// ut
//
#include <ut/algo.hpp>
#include <ut/check.hpp>
using namespace ut;

//
// std
//
using namespace std;


#include "rlgl.h"

//
// Helper Functions
//

static color nextColor()
{
    static auto c = color::hsluv{120, 80, 80};
    c.rotate(100);
    return c.toColor();
}

void drawOverlayOutline(rect const& rect, color const &c, float thickness)
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

void drawOverlayFrame(rect const& outer, rect const& inner, color const& c)
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

void drawOverlayTextSelected(rect const& rect, color const& c, cstrparam text)
{
    using namespace ImGui;

    auto dl = GetBackgroundDrawList();

    auto sz         = vec2( CalcTextSize(text.begin(), text.end()) );
    auto sz2        = sz/2;
    auto rr         = rect.round();
    auto rr_bg      = rr.anchorCCtoCC(sz).inflated(sz2.y);
    auto pos        = rr.center() - sz2;

    dl->AddRectFilled(rr_bg.min, rr_bg.max, ToU32(c));
    dl->AddText(pos, 0xff000000, text.begin(), text.end());
}

void drawOverlayTextUnselected(rect const& rect, color const& c, cstrparam text)
{
    using namespace ImGui;

    auto dl = GetBackgroundDrawList();

    auto sz         = vec2( CalcTextSize(text.begin(), text.end()) );
    auto rr         = rect.round();
    auto pos        = rr.center() - (sz/2);
    dl->AddText(pos, ToU32(c), text.begin(), text.end());
}

void drawOverlayCursor(vec2 const& mp)
{
    using namespace ImGui;

    auto dl = GetBackgroundDrawList();

    auto t = mp.withOffsetY(-10);
    auto b = mp.withOffsetY(10);
    auto l = mp.withOffsetX(-10);
    auto r = mp.withOffsetX(10);

    dl->AddLine(t,b, 0xff0000ff);
    dl->AddLine(l,r, 0xff0000ff);

}

//
// Box -> Implementation
//

Box::Box(box_ptr p) :
    name            { },
    parent          { std::move(p) },
    m_color         { nextColor() },
    m_want_bind     { true }
{}

void Box::notifyChanged()
{
    auto& v = BoxVisitor::currentInstance();
    v.want_persist = true;
    setWantBindAll();
}

void Box::setWantBindAll()
{
    m_want_bind = true;
    for (auto&& it : child_boxes)
        it->setWantBindAll();
}

void Box::clearWantBind()
{
    m_want_bind = false;
}

bool Box::wantBind() const
{
    return m_want_bind;
}

bool Box::isRoot(box_ptr const& box)
{
    return box && box->parent == nullptr;
}

box_ptr Box::create(box_ptr const& parent)
{
    check_null(parent);
    return box_ptr{new Box(parent)};
}

box_ptr Box::ptr()
{
    return shared_from_this();
}

box_ptr Box::deepCopy(box_ptr const& p)
{
    box_ptr box = box_ptr{new Box(p)};

    box->name           = name;
    box->flex           = flex;
    box->sizer          = sizer;
    box->weight         = weight;
    box->lb             = lb;

    box->m_child_action = m_child_action;
    box->m_want_bind = m_want_bind;
    box->m_color = m_color;

    for (auto&& it : child_boxes)
    {
        check_null(it);
        box->child_boxes.push_back(it->deepCopy(box));
    }

    return box;
}

void Box::mutate(box_ptr const& original)
{
    flex        = original->flex;
    sizer       = original->sizer;
    weight      = original->weight;

    child_boxes.resize(original->child_boxes.size());
    for (size_t i = 0; i < child_boxes.size(); ++i)
    {
        auto&& it = child_boxes[i];
        if (!it)
            it = box_ptr{new Box{ptr()}};
        it->mutate(original->child_boxes[i]);
    }
}

box_ptr Box::tryGetBox(vec2 const& mp)
{
    for (auto&& it: child_boxes)
    {
        if (auto box = it->tryGetBox(mp))
            return box;
    }

    if (auto r = lb.border; r.contains(mp))
        return ptr();
    return nullptr;
}

string Box::getLbl() const
{
    if (name.empty())
    {
        switch (flex.type)
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
    auto& v = BoxVisitor::currentInstance();

    assert(v.isBoxSelectedSingle(ptr()));

    Text("Selection");

    BeginGroup();
    if (parent)
    {
        if (Button("to Parent"))
        {
            v.setSelectedBoxSingle(parent);
        }

        SameLine();

        if (Button("to Multi"))
        {
            v.setMutateSelection(ptr());
        }

        SameLine();

        if (ButtonConfirm("Delete"))
            parentActionDelete();

        SameLine();

        if (ButtonEnabled("Mutate", v.canMutate()))
        {
            for (auto&& it: v.boxSelectionMulti())
                it->mutate(ptr());
            parent->notifyChanged();
        }

        SameLine();

        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        if (Button("--"))
            parentActionMoveDec();

        SameLine();

        if (Button("++"))
            parentActionMoveInc();

        PopStyleVar();
    }
    else
    {
        PushItemDisabled();
        Button("to Parent");
        SameLine();
        Button("Delete");
        PopItemDisabled();
    }
    EndGroup();

    Separator();

    ButtonEnabled(" ", false);
    SameLine();
    LabelText("Outer", "%s", to_string(lb.outer.cast<int>().psize()).c_str());
    ButtonEnabled(" ", false);
    SameLine();
    LabelText("Border", "%s", to_string(lb.border.cast<int>().psize()).c_str());
    ButtonEnabled(" ", false);
    SameLine();
    LabelText("Inner", "%s", to_string(lb.inner.cast<int>().psize()).c_str());

    ButtonEnabled(" ", false);
    SameLine();
    drawBreadcrumbs();


    if (ButtonDefault("Name", !name.empty()))
    { v.resetBoxSlot(ptr()); }

    PushStyleColor(ImGuiCol_Text, ToU32(m_color));

    if (BeginCombo("Name###name_get", name.c_str()))
    {
        for (auto&& it : v.boxMap())
        {
            auto key = it.first;
            auto box = it.second;

            auto lbl = PRINTER(box ? "o %s" : "- %s", key.c_str());
            auto col = box ? ToU32(box->m_color) : 0xffffffff;

            PushStyleColor(ImGuiCol_Text, col);
            if (Selectable(lbl, name == key))
            {
                v.resetBoxSlot(ptr());
                name = key;
                v.trySetBoxSlot(ptr());

                notifyChanged();
            }
            PopStyleColor();
        }

        EndCombo();
    }

    PopStyleColor();

    Separator();

    Text("Size and Position");

    if (sizer.drawProperties())
        notifyChanged();

    Separator();

    Text("Flex");

    if (flex.drawProperties())
        notifyChanged();

    Separator();

    Text("Container");

    if (parent)
    {
        if (Button("U"))
        {
            parent->calcWeightsUniform();
        }
        if (IsItemHovered())
            SetTooltip("Uniform Distribution");
        SameLine();

        BeginGroup();

        if (Button("A"))
        {
            parent->calcWeightsUniformAnchor(ptr());
        }
        if (IsItemHovered())
            SetTooltip("Uniform Distribution with Anchor");
        SameLine();

        if (float w = weight; SliderFloat("Weight", &w, 0, 1))
        {
            if (w > 0 && w < 1)
            {
                weight = w;
                notifyChanged();
                parent->calcWeightsPropAnchor(ptr());
            }
        }

        EndGroup();
    }


    if (ButtonDefault("child_boxes", !child_boxes.empty()))
    { child_boxes.clear(); }

    if (Button("add"))
        insertChildEnd();

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

        bool show_row_select = v.show_row_select;
        v.show_row_select = false;
        for (auto&& it: child_boxes)
            it->drawTreeTableRow(true);
        v.show_row_select = show_row_select;

        EndTable();
    }
}




void Box::drawTreeTableRow()
{
    using namespace ImGui;

    if (child_boxes.empty())
    {
        drawTreeTableRow(true);
    }
    else if (drawTreeTableRow(false))
    {
        for (auto&& it: child_boxes)
            it->drawTreeTableRow();
        TreePop();
    }
}

bool Box::drawTreeTableRow(bool is_leaf)
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    bool is_open = false;
    bool is_visitor_active = v.isActive();
    if (!is_visitor_active)
        PushItemDisabled();
    PushID(this);

    TableNextRow();

    //
    // TreeNode
    //

    if (TableNextColumn())
    {
        PushStyleColor(ImGuiCol_Text, m_color);

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

        if (is_leaf && IsItemClicked(ImGuiMouseButton_Left))
        {
            if (IsKeyDown(ImGuiKey_LeftCtrl))
                v.toggleSelectedBoxMulti(ptr());
            else
                v.setSelectedBoxSingle(ptr());
        }

        PopStyleColor();
    }

    //
    // Controls
    //

    PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{3.0f,0.0f});

    if (TableNextColumn())
    {
        auto const& edit_opts = LEDIT_OPTIONS.box_edit_options;

        if (v.show_row_select)
        {
            if (v.drawSelectionButton(ptr()))
            {
                if (IsKeyDown(ImGuiKey_LeftCtrl))
                    v.toggleSelectedBoxMulti(ptr());
                else
                    v.setSelectedBoxSingle(ptr());
            }
            SameLine();
        }

        if (edit_opts.show_row_add)
        {
            if (SmallButton("+"))
                insertChildEnd();
            SameLine();
        }

        if (parent && edit_opts.show_row_delete)
        {
            if (SmallButtonConfirm("x"))
                parentActionDelete();
            SameLine();
        }

        PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        if (parent && edit_opts.show_row_move)
        {
            if (SmallButton("--"))
                parentActionMoveDec();
            SameLine();

            if (SmallButton("++"))
                parentActionMoveInc();
            SameLine();
        }

        PopStyleVar();

        if (edit_opts.show_row_rename)
        {
            if (SmallButton("n"))
                OpenPopup("show-row-rename");

            if (BeginPopup("show-row-rename"))
            {
                std::array<char, 20> static name_buffer;
                ::strncpy(name_buffer.data(), name.c_str(), name_buffer.size());
                if (InputText("Name", name_buffer.data(), name_buffer.size()))
                    name = name_buffer.data();
                EndPopup();
            }
            SameLine();
        }

        flex.drawRowControls();


    }

    PopStyleVar(2);

    PopID();
    if (!is_visitor_active)
        PopItemDisabled();

    return is_open;
}

void Box::drawOverlaySingleSelectedBelow()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    auto o = v.getRealRect(lb.outer);
    auto b = v.getRealRect(lb.border);
    auto i = v.getRealRect(lb.inner);

    drawOverlayFrame(o, b, m_color.withNormalA(.25f));
    drawOverlayFrame(b, i, m_color.withNormalA(.50f));
}

void Box::drawOverlaySingleSelectedAbove()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    auto o = v.getRealRect(lb.border);
    auto c = m_color;
    drawOverlayOutline(o, c, 3.0f);

    if (!name.empty())
        drawOverlayTextSelected(o, c, name);
}

void Box::drawOverlayOutlines()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    auto& overlay_opts = LEDIT_OPTIONS.overlay_options;

    if (!v.isBoxSelectedSingle(ptr()))
    {
        bool is_multi = v.isBoxSelectedMulti(ptr());

        auto o = v.getRealRect(lb.border);
        auto c = is_multi ? overlay_opts.style().border.opaque() : overlay_opts.style().border;
        auto t = is_multi ? 3.0f : 1.0f;

        drawOverlayOutline(o, c, t);

        if (!name.empty())
            drawOverlayTextUnselected(o, c, name);
    }

    for (auto&& it: child_boxes)
    {
        it->drawOverlayOutlines();
    }
}

void Box::reset()
{
    name = "";
    flex.reset();
    sizer.reset();
    child_boxes.clear();
}

void Box::normalizeWeights()
{
    calcWeightsNormal();
    for (auto&& it : child_boxes)
        it->normalizeWeights();
}

string Box::toYamlString()
{
    YAML::Emitter em;
    emitYaml(em, ptr());
    return em.c_str();
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
            //normalizeWeights();
            if (parent)
                calcLayout(parent->lb.inner);
            else
                calcLayout(rect{});
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

//
// layout
//

void Box::calcLayout(rect const& p)
{
    sizer.getBoxRects(p, lb);

    if (child_boxes.empty())
        return;

    switch (flex.type)
    {
        case BOX_VBOX:
            calcLayoutVbox(lb.inner);
            break;
        case BOX_HBOX:
            calcLayoutHbox(lb.inner);
            break;
        case BOX_SBOX:
            calcLayoutSbox(lb.inner);
            break;
        default:
            nopath_case(BoxType);
    }
}

void Box::calcLayoutVbox(rect const& b)
{
    auto sz     = child_boxes.size();
    auto h      = b.height() - (flex.inner_pad * float(sz - 1));
    auto cl     = b.min.x;
    auto cr     = b.max.x;

    auto cy = b.min.y;

    for (size_t i = 0; i < sz - 1; ++i)
    {
        auto &&it = child_boxes[i];
        auto ch = h * it->weight;
        auto ct = cy;
        auto cb = cy + ch;

        it->calcLayout({cl, ct, cr, cb});

        cy = cb + flex.inner_pad;
    }

    child_boxes.back()->calcLayout({cl, cy, cr, b.max.y});
}

void Box::calcLayoutHbox(rect const& b)
{
    auto sz     = child_boxes.size();
    auto w      = b.width() - (flex.inner_pad * float(sz - 1));
    auto ct     = b.min.y;
    auto cb     = b.max.y;

    auto cx = b.min.x;

    for (size_t i = 0; i < sz - 1; ++i)
    {
        auto &&it = child_boxes[i];
        auto cw = w * it->weight;
        auto cl = cx;
        auto cr = cx + cw;

        it->calcLayout({cl, ct, cr, cb});

        cx = cr + flex.inner_pad;
    }

    child_boxes.back()->calcLayout({cx, ct, b.max.x, cb});
}

void Box::calcLayoutSbox(rect const &b)
{
    for (auto &&it: child_boxes)
    {
        it->calcLayout(b);
    }
}

//
// weight calculation
//

void Box::calcWeightsPropAnchor(box_ptr const& anchor)
{
    if (child_boxes.size() > 1)
    {
        float target    = 1.0f - anchor->weight;
        float sum       = 0.0f;
        for (auto&& it : child_boxes)
            if (it != anchor)
                sum += it->weight;

        if (sum > 0.0f)
        {
            for (auto&& it : child_boxes)
            {
                if (it != anchor)
                {
                    it->weight *= target / sum;
                    it->notifyChanged();
                }
            }
        }
    }
    else if (!child_boxes.empty())
    {
        anchor->weight = 1.0f;
        anchor->notifyChanged();
    }
}

void Box::calcWeightsUniformAnchor(box_ptr const& anchor)
{
    if (child_boxes.size() > 1)
    {
        float target    = 1.0f - anchor->weight;
        float w         = target / float(child_boxes.size()-1);
        for (auto&& it: child_boxes)
        {
            if (it != anchor)
            {
                it->weight = w;
                it->notifyChanged();
            }
        }
    }
    else if (!child_boxes.empty())
    {
        anchor->weight = 1.0f;
        anchor->notifyChanged();
    }
}

void Box::calcWeightsUniform()
{
    if (!child_boxes.empty())
    {
        float w = 1.0f / float(child_boxes.size());
        for (auto&& it: child_boxes)
        {
            it->weight = w;
            it->notifyChanged();
        }
    }
}

void Box::calcWeightsNormal()
{
    if (!child_boxes.empty())
    {
        float sum = 0.0f;
        for (auto&& it : child_boxes)
            sum += it->weight;

        if (sum > 0.0f)
        {
            for (auto&& it : child_boxes)
            {
                it->weight /= sum;
                it->notifyChanged();
            }
        }
        else
        {
            float w = 1.0f / float(child_boxes.size());
            for (auto&& it : child_boxes)
            {
                it->weight = w;
                it->notifyChanged();
            }
        }
    }
}

//
// child action
//

void Box::insertChildEnd()
{
    insertChild(child_boxes.end(), create(ptr()));
}

void Box::insertChildStart()
{
    insertChild(child_boxes.begin(), create(ptr()));
}

void Box::insertChild(boxlist_t::iterator const& pos, box_ptr const& box)
{
    child_boxes.insert(pos, box);
    calcWeightsUniform();
}

void Box::parentActionDelete()
{
    auto& v = BoxVisitor::currentInstance();

    check_null(parent);
    v.setSelectedBoxSingle(parent);
    v.resetBoxSlot(ptr());
    parent->m_child_action = {ChildAction::DELETE, ptr()};
}

void Box::parentActionClone()
{
    auto& v = BoxVisitor::currentInstance();

    check_null(parent);
    parent->m_child_action = {ChildAction::CLONE, ptr()};
}

void Box::parentActionMoveInc()
{
    auto& v = BoxVisitor::currentInstance();

    check_null(parent);
    parent->m_child_action = {ChildAction::MOVE_INC, ptr()};
}

void Box::parentActionMoveDec()
{
    auto& v = BoxVisitor::currentInstance();

    check_null(parent);
    parent->m_child_action = {ChildAction::MOVE_DEC, ptr()};
}

void Box::applyChildActions()
{
    if (m_child_action)
    {
        auto b  = child_boxes.begin();
        auto e  = child_boxes.end();
        auto ca = *m_child_action;

        m_child_action.reset();

        if (auto it = find(b, e, ca.box); it != e)
        {
            switch (ca.type)
            {
                case ChildAction::DELETE:
                    child_boxes.erase(it);
                    calcWeightsNormal();
                    break;

                case ChildAction::MOVE_DEC:
                    if (it != b)
                        it->swap(*(it - 1));
                    break;

                case ChildAction::MOVE_INC:
                    if (auto down = it + 1; down != e)
                        it->swap(*down);
                    break;

                case ChildAction::CLONE:
                    // TODO: decide what to do about weights
                    // child_boxes.insert(it+1, (*it)->deepCopy(ptr()));
                    nopath_impl;
                    break;

                default:nopath_case(ChildAction::Type);
            }

            for (auto&& box : child_boxes)
                box->notifyChanged();
        }
        else
        {
            nopath("didn't find child action box");
        }
    }

    for (auto && it : child_boxes)
        it->applyChildActions();
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

void Box::drawBreadcrumbs()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    box_ptr b = ptr();
    vector<box_ptr> ps;

    do
    {
        ps.push_back(b);
        b = b->parent;
    } while (b);

    BeginGroup();
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

    auto it = ps.rbegin();

    PushID(it->get());
    if (SmallButton((*it)->getLbl(), WidgetColors::fromHSLUVAlt((*it)->m_color.toHSLUV())))
        v.setSelectedBoxSingle(*it);
    PopID();

    for (++it; it != ps.rend(); ++it)
    {
        SameLine();

        PushID(it->get());

        auto lbl = PRINTER(ICON_LC_ARROW_RIGHT " %s", (*it)->getLbl().c_str());
        auto wc  = WidgetColors::fromHSLUVAlt((*it)->m_color.toHSLUV());
        if (SmallButton(lbl, wc))
            v.setSelectedBoxSingle(*it);

        PopID();
    }

    PopStyleVar();
    EndGroup();
}

void Box::drawOverlay()
{
    auto& v = BoxVisitor::currentInstance();

    check(isRoot(v.root_box), "invalid root");

    using namespace ImGui;

    if (!v.is_overlay_visible)
        return;

    auto& overlay_opts = LEDIT_OPTIONS.overlay_options;

    v.root_box->applyChildActions();
    v.root_box->calcLayout(rect{});

    {
        auto rr = v.getRealRect(v.root_box->lb.outer).round();
        auto dl = GetBackgroundDrawList();
        auto bg = ToU32(overlay_opts.style().background);

        dl->AddRectFilled(rr.min, rr.max, bg);
    }

    if (auto selected_box = v.boxSelectionSingle())
    {
        selected_box->drawOverlaySingleSelectedBelow();
        v.root_box->drawOverlayOutlines();
        selected_box->drawOverlaySingleSelectedAbove();
    }
    else
    {
        v.root_box->drawOverlayOutlines();
    }

    auto& io = GetIO();

    if (!io.WantCaptureKeyboard)
    {
        auto& idx = overlay_opts.style_index;
        auto count = int(overlay_opts.styles.size());

        if (IsKeyPressed(ImGuiKey_Q))
            idx = (idx-1 % count + count) % count;
        if (IsKeyPressed(ImGuiKey_W))
            idx = (idx+1 % count + count) % count;
        if (IsKeyPressed(ImGuiKey_E))
            v.want_capture_mouse = !v.want_capture_mouse;
    }

    if (!io.WantCaptureMouse && v.want_capture_mouse)
    {
        auto mp = v.getViewPoint(GetMousePos());

        if (IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (auto box = v.root_box->tryGetBox(mp))
            {
                if (IsKeyDown(ImGuiKey_LeftCtrl))
                {
                    v.toggleSelectedBoxMulti(box);
                }
                else
                {
                    v.setSelectedBoxSingle(box);
                }

            }
            else
            {
                v.clearSelectedBoxSingle();
                v.is_properties_window_open = false;
            }
        }

        if (IsMouseClicked(ImGuiMouseButton_Middle))
        {
            if (auto selected_box = v.boxSelectionSingle())
                if (auto parent_box = selected_box->parent)
                    v.setSelectedBoxSingle(parent_box);
        }

        if (IsMouseClicked(ImGuiMouseButton_Right))
        {
            if (auto box = v.root_box->tryGetBox(mp))
            {
                v.setSelectedBoxSingle(box);
                v.is_properties_window_open = true;

                //SetNextWindowPos(GetMousePos());
            }
            else
            {
                v.clearSelectedBoxSingle();
                v.is_properties_window_open = false;
            }
        }

        drawOverlayCursor(GetMousePos());
    }
}

void Box::drawBoxHierarchy()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    check(isRoot(v.root_box), "invalid root");

    ImGuiTableFlags table_flags =
        ImGuiTableFlags_BordersV        |
        ImGuiTableFlags_BordersOuterH   |
        ImGuiTableFlags_Resizable       |
        ImGuiTableFlags_RowBg           |
        ImGuiTableFlags_NoBordersInBody;

    if (BeginTable("grids", 2, table_flags))
    {
        TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
        TableSetupColumn("Controls", ImGuiTableColumnFlags_WidthFixed);
        TableHeadersRow();

        v.root_box->drawTreeTableRow();

        EndTable();
    }
}

void Box::drawPropertiesWindow()
{
    using namespace ImGui;
    auto& v = BoxVisitor::currentInstance();

    if (!v.is_overlay_visible)
        return;

    if (auto selected_box = v.boxSelectionSingle(); selected_box && v.is_properties_window_open)
    {
        auto lbl = PRINTER("Box Properties [%s]###selected_box_popup_%s",
                           v.name().c_str(), v.name().c_str());

        auto window_flags =
            ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoDocking;

        if (Begin(lbl, &v.is_properties_window_open, window_flags))
        {
            selected_box->drawProperties();
        }
        End();
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
