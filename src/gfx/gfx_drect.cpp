#include "gfx/gfx_drect.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"
using namespace gfx;

#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

//
// Helper Functions
//
static color nextColor()
{
    static auto c = color::hsluv{120, 80, 80};
    c.rotate(100);
    return c.toColor();
}

//
// DebugRectManager -> Implementation
//

DebugRectOverlayManager::DebugRectOverlayManager() :
    m_root_tag      {true, false, "", {}, 0},
    m_label         {""},
    m_draws         { },
    m_im_style      {ImGuiDRECTStyle_NoText}
{}

DebugRectOverlayManager& DebugRectOverlayManager::instance()
{
    static DebugRectOverlayManager x;
    return x;
}

void DebugRectOverlayManager::addRect(cstrparam label, rect const& r)
{
    assert(!m_label.empty());

    m_label.back() = label;
    addRect(r);
    m_label.back() = "";
}

void DebugRectOverlayManager::addRect(rect const& r)
{
    assert(!m_label.empty());

    if (!enabled)
        return;

    Tag& tag = m_root_tag.getChildTag(m_label.data(), m_label.data() + m_label.size());

    tag.overlay_count++;
    if (tag.enabled | tag.highlighted)
        m_draws.push_back(tag.toOverlay(r));
}

void DebugRectOverlayManager::pushRect(cstrparam label, rect const& r)
{
    m_label.back() = label;
    addRect(r);
    m_label.emplace_back("");
}

void DebugRectOverlayManager::popRect()
{
    assert(m_label.size() > 1);
    m_label.pop_back();
}

void DebugRectOverlayManager::drawDebug()
{
    using namespace ImGui;

    for (auto&& it: m_draws)
    {
        auto c = it.color.withNormalA(it.highlighted ? 0.5f : 0.0f);
        auto f = it.highlighted ? m_im_style : ImGuiDRECTStyle_NoText;
        auto r = view_transform ? view_transform->realRect(it.bound) : it.bound;
        DrawDRECT(it.text, r, c, f);
    }

    m_draws.clear();

    if (enabled)
    {
        auto tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
        enabled = ImGui::TreeNodeEx("Debug Rectangles", tree_flags);
    }
    else
    {
        enabled = ImGui::TreeNodeEx("Debug Rectangles", ImGuiTreeNodeFlags_Framed);
    }

    if (enabled)
    {
        RadioButton("NoText", &m_im_style, ImGuiDRECTStyle_NoText);
        SameLine(125);
        RadioButton("LabelOnly", &m_im_style, ImGuiDRECTStyle_LabelOnly);

        RadioButton("ValueOnly", &m_im_style, ImGuiDRECTStyle_ValueOnly);
        SameLine(125);
        RadioButton("FullText", &m_im_style, ImGuiDRECTStyle_FullText);

        if (Button("Enable All"))
        {
            m_root_tag.enableAll();
        }

        SameLine();

        if (Button("Disable All"))
        {
            m_root_tag.disableAll();
        }

        ImGuiTableFlags table_flags =
                ImGuiTableFlags_BordersV        |
                ImGuiTableFlags_BordersOuterH   |
                ImGuiTableFlags_Resizable       |
                ImGuiTableFlags_RowBg           |
                ImGuiTableFlags_NoBordersInBody;

        if (BeginTable("rectangles", 2, table_flags))
        {
            TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
            TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            TableHeadersRow();

            for (auto&& it : m_root_tag.child_tags)
                it.draw();

            EndTable();
        }

        TreePop();
    }
}

//
// DebugRectManager -> RectTag -> Implementation
//

void DebugRectOverlayManager::Tag::enableAll()
{
    enabled = true;
    for (auto&& it : child_tags)
        it.enableAll();
}

void DebugRectOverlayManager::Tag::disableAll()
{
    enabled = false;
    for (auto&& it : child_tags)
        it.disableAll();
}

void DebugRectOverlayManager::Tag::draw()
{
    if (child_tags.empty())
    {
        draw(true);
    }
    else if (draw(false))
    {
        for (auto&& it : child_tags)
            it.draw();
        ImGui::TreePop();
    }
}

bool DebugRectOverlayManager::Tag::draw(bool is_leaf)
{
    using namespace ImGui;

    TableNextRow();
    TableNextColumn();
    PushID(this);

    int flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if (is_leaf)
    {
        flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    bool open = ImGui::TreeNodeEx(PRINTER("[%d] ", (int)overlay_count), flags);

    highlighted = ImGui::IsItemHovered();

    SameLine();
    PushStyleColor(ImGuiCol_Text, color);
    Text("%s", text.c_str());
    PopStyleColor();

    TableNextColumn();
    PushID(text);
    PushStyleColor(ImGuiCol_Button, enabled ? colors::greenyellow : colors::orangered);
    if (SmallButton(enabled ? " " : "x"))
    {
        enabled = !enabled;
        if (enabled)
            enableAll();
        else
            disableAll();
    }
    PopStyleColor();
    PopID();

    overlay_count = 0;

    PopID();

    return open;
}



DebugRectOverlayManager::Tag& DebugRectOverlayManager::Tag::getChildTag(cstrview const* begin, cstrview const* end)
{
    assert(begin <= end);

    if (begin == end)
        return *this;

    auto key = *begin;
    for (auto&& it: child_tags)
    {
        if (it.text == key)
            return it.getChildTag(begin+1, end);
    }

    child_tags.push_back({true, false, key, nextColor(), 0});
    return child_tags.back();
}

#if 0
void DebugRectManager::RectTag::drawLeaf()
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::PushID(this);

    ImGui::TreeNodeEx(PRINTER("[%d] ", (int)count), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

    highlighted = ImGui::IsItemHovered();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Text("%s", text.c_str());
    ImGui::PopStyleColor();

    ImGui::TableNextColumn();

    ImGui::PushID(text);
    ImGui::PushStyleColor(ImGuiCol_Button, enabled ? colors::greenyellow : colors::orangered);
    if (ImGui::SmallButton(enabled ? " " : "x"))
    {
        enabled = !enabled;
        if (enabled)
            enableAll();
        else
            disableAll();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();

    count = 0;

    ImGui::PopID();
}

void DebugRectManager::RectTag::drawBranch()
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::PushID(this);

    bool open = ImGui::TreeNodeEx(PRINTER("[%d] ", (int)count), ImGuiTreeNodeFlags_SpanFullWidth);

    highlighted = ImGui::IsItemHovered();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, color);
    ImGui::Text("%s", text.c_str());
    ImGui::PopStyleColor();

    ImGui::TableNextColumn();

    ImGui::PushID(text);
    ImGui::PushStyleColor(ImGuiCol_Button, enabled ? colors::greenyellow : colors::orangered);
    if (ImGui::SmallButton(enabled ? " " : "x"))
    {
        enabled = !enabled;
        if (enabled)
            enableAll();
        else
            disableAll();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();

    count = 0;

    if (open)
    {
        for (auto&& it : child_tags)
            it.draw();
        ImGui::TreePop();
    }

    ImGui::PopID();
}

struct Gizmo
{
    enum Mode
    {
        MODE_DISABLED,
        MODE_PAN,
        MODE_ZOOM
    } mode = MODE_DISABLED;

    enum Dragging
    {
        DRAG_NONE,
        DRAG_LEFT,
        DRAG_RIGHT
    } drag = DRAG_NONE;

    vec2 offset;
    vec2 offset_prev;

    float zoom = 1;
    float zoom_prev = 1;


    void drawModePanning()
    {
        vec2 mouse_pos      = ImGui::GetMousePos();
        vec2 mouse_delta    = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);

        ImGui::GetBackgroundDrawList()->AddCircle(mouse_pos, 30, ImGui::ColorConvertFloat4ToU32(colors::white));

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            ImGui::GetBackgroundDrawList()->AddCircle(mouse_pos - mouse_delta, 30, ImGui::ColorConvertFloat4ToU32(colors::limegreen));
            offset = offset_prev + mouse_delta;
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            offset_prev = offset;
        }

    }

    void drawModeZooming()
    {
        vec2 mouse_pos = ImGui::GetMousePos();


        ImGui::GetBackgroundDrawList()->AddCircle(mouse_pos, 30, ImGui::ColorConvertFloat4ToU32(colors::white));

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            vec2 mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            ImGui::GetBackgroundDrawList()->AddCircle(mouse_pos - mouse_delta, 30, ImGui::ColorConvertFloat4ToU32(colors::pink));

            zoom = zoom_prev + log(mouse_delta.length());
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            zoom_prev = zoom;
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Right))
        {
            vec2 mouse_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            ImGui::GetBackgroundDrawList()->AddCircle(mouse_pos - mouse_delta, 30, ImGui::ColorConvertFloat4ToU32(colors::orangered));

            zoom = zoom_prev + mouse_delta.length();
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            zoom_prev = zoom;
        }
    }

    void draw()
    {
        if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_Q))
        {
            mode = mode == MODE_DISABLED ? MODE_PAN : MODE_DISABLED;
            return;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_LeftShift) && mode == MODE_PAN)
        {
            mode = MODE_ZOOM;
            return;
        }

        if (ImGui::IsKeyReleased(ImGuiKey_LeftShift) && mode == MODE_ZOOM)
        {
            mode = MODE_PAN;
            return;
        }

        if (mode == MODE_PAN)
            drawModePanning();

        if (mode == MODE_ZOOM)
            drawModeZooming();
    }

} static GIZMO;
#endif