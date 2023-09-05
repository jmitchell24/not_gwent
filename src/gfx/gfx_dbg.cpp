#include "gfx/gfx_dbg.hpp"
#include "random.hpp"
#include "rlImGui/imgui/imgui_extra.hpp"
using namespace gfx;

using namespace ut;

//
// DebugRectManager -> Implementation
//

DebugRectManager::DebugRectManager() :
    m_tags      {},
    m_draws     {},
    m_im_style  {ImGuiDebugRectangleStyle_Default},
    m_im_alpha  {1}
{}

void DebugRectManager::addRect(cstrparam label, rectf const& r)
{
    if (!enabled)
        return;

    RectTag& tag = getTag(label);

    tag.cnt++;

    if (tag.enabled)
        m_draws.push_back(tag.toDraw(r));
}

bool DebugRectManager::drawDebug()
{
    for (auto& p: m_draws)
        ImGui::DrawDebugRectangle(p.text, p.bound, p.col.withNormalA(m_im_alpha), (ImGuiDebugRectangleStyle_)m_im_style);
    m_draws.clear();

    auto flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::TreeNodeEx("Debug Rectangles", flags))
    {
        if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent))
            enabled = !enabled;



        ImGui::Checkbox("Enable Debug", &enabled);


        ImGui::Separator();
        ImGui::Text("Bounds");


        if (ImGui::Button("100%")) m_im_alpha = 1.00f;
        ImGui::SameLine();
        if (ImGui::Button("75%")) m_im_alpha = 0.75f;
        ImGui::SameLine();
        if (ImGui::Button("50%")) m_im_alpha = 0.50f;
        ImGui::SameLine();
        if (ImGui::Button("25%")) m_im_alpha = 0.25f;

        ImGui::DragFloat("Alpha", &m_im_alpha, 0.003, 0.25, 1);
        ImGui::RadioButton("Default", &m_im_style, ImGuiDebugRectangleStyle_Default);
        ImGui::SameLine();
        ImGui::RadioButton("Full", &m_im_style, ImGuiDebugRectangleStyle_Full);
        ImGui::SameLine();
        ImGui::RadioButton("Simple", &m_im_style, ImGuiDebugRectangleStyle_Simple);

        if (ImGui::Button("Enable All"))
        {
            for (auto& p: m_tags)
                p.enabled = true;
        }

        ImGui::SameLine();

        if (ImGui::Button("Disable All"))
        {
            for (auto& p: m_tags)
                p.enabled = false;
        }

        ImGui::BeginChild("virt2d-rects", {0,150}, true);

        for (auto& p: m_tags)
        {
            auto lbl = p.text.c_str();
            auto nor = p.col.toNormal();

            ImGui::Text("%d", (int)p.cnt);

            ImGui::SameLine();

            if (float c[3]{nor.r, nor.g, nor.b}; ImGui::ColorEdit3(lbl, c, ImGuiColorEditFlags_NoLabel|ImGuiColorEditFlags_NoInputs))
                p.col = color::fromNormal({c[0], c[1], c[2]});

            ImGui::SameLine();

            ImGui::Checkbox(lbl, &p.enabled);

            p.cnt = 0;
        }

        ImGui::EndChild();

        ImGui::TreePop();

        return true;
    }

    return false;
}

DebugRectManager::RectTag& DebugRectManager::getTag(cstrparam label)
{
    for (RectTag& x: m_tags)
        if (x.text == label)
            return x;
    m_tags.push_back({true, label, RANDOM.nextColor(),0});
    return m_tags.back();
}

#if 0
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