#include "gfx/gfx_dbg.hpp"
#include "rlImGui/imgui/imgui_extra.hpp"
using namespace gfx;

using namespace ut;

//
// DebugRectManager -> Implementation
//

DebugRectManager::DebugRectManager() :
    m_root_tag      {true, false, "", {}, 0},
    m_label         {""},
    m_draws         { },
    m_im_style      {ImGuiDebugRectangleStyle_Simple}
{}

DebugRectManager& DebugRectManager::instance()
{
    static DebugRectManager x;
    return x;
}

void DebugRectManager::addRect(cstrparam label, rectf const& r)
{
    assert(!m_label.empty());

    m_label.back() = label;
    addRect(r);
    m_label.back() = "";
}

void DebugRectManager::addRect(rectf const& r)
{
    assert(!m_label.empty());

    if (!enabled)
        return;

    Tag& tag = m_root_tag.getChildTag(m_label.data(), m_label.data() + m_label.size());

    tag.overlay_count++;
    if (tag.enabled | tag.highlighted)
        m_draws.push_back(tag.toOverlay(r));
}

void DebugRectManager::pushRect(cstrparam label, rectf const& r)
{
    m_label.back() = label;
    addRect(r);
    m_label.emplace_back("");
}

void DebugRectManager::popRect()
{
    assert(m_label.size() > 1);
    m_label.pop_back();
}

void DebugRectManager::drawDebug()
{
    for (auto& p: m_draws)
    {
        auto draw_flags = p.highlighted ? ImGuiDebugRectangleStyle_Full : (ImGuiDebugRectangleStyle_)m_im_style;
        ImGui::DrawDebugRectangle(p.text, p.bound, p.color.withNormalA(0.5f), draw_flags);
    }

    m_draws.clear();

    auto tree_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
    enabled = ImGui::TreeNodeEx("Debug Rectangles", tree_flags);

    if (enabled)
    {
        ImGui::RadioButton("Default", &m_im_style, ImGuiDebugRectangleStyle_Default);
        ImGui::SameLine();
        ImGui::RadioButton("No Label", &m_im_style, ImGuiDebugRectangleStyle_Simple);
        ImGui::SameLine();

        if (ImGui::Button("Enable All"))
        {
            m_root_tag.enableAll();
        }

        ImGui::SameLine();

        if (ImGui::Button("Disable All"))
        {
            m_root_tag.disableAll();
        }

        float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
        ImGuiTableFlags table_flags =
                ImGuiTableFlags_BordersV        |
                ImGuiTableFlags_BordersOuterH   |
                ImGuiTableFlags_Resizable       |
                ImGuiTableFlags_RowBg           |
                ImGuiTableFlags_NoBordersInBody;


        if (ImGui::BeginTable("rectangles", 2, table_flags))
        {
            // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
            ImGui::TableSetupColumn("Label"     , ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Enabled"   , ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
            ImGui::TableHeadersRow();

            for (auto&& it : m_root_tag.child_tags)
                it.draw();

            ImGui::EndTable();
        }

        ImGui::TreePop();
    }
}

//
// DebugRectManager -> RectTag -> Implementation
//

void DebugRectManager::Tag::enableAll()
{
    enabled = true;
    for (auto&& it : child_tags)
        it.enableAll();
}

void DebugRectManager::Tag::disableAll()
{
    enabled = false;
    for (auto&& it : child_tags)
        it.disableAll();
}

void DebugRectManager::Tag::draw()
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

bool DebugRectManager::Tag::draw(bool is_leaf)
{
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::PushID(this);

    int flags = ImGuiTreeNodeFlags_SpanFullWidth;
    if (is_leaf)
    {
        flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    bool open = ImGui::TreeNodeEx(PRINTER("[%d] ", (int)overlay_count), flags);

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

    overlay_count = 0;

    ImGui::PopID();

    return open;
}

static color nextColor()
{
    static size_t counter=0;

    auto hue = float(counter++) * 100.0f + 120.0f;
    hue = fmodf(hue, 360.0f);

    return color(color::hsluv{hue, 80.0f, 80.0f, 1.0f});
}

DebugRectManager::Tag& DebugRectManager::Tag::getChildTag(cstrview const* begin, cstrview const* end)
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