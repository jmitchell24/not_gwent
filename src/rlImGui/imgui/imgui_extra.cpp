#include "imgui_extra.hpp"
#include "rlImGui/imgui/imgui.h"
#include "rlImGui/imgui/imgui_internal.h"

#include <ut/color.hpp>
using namespace ut;

//
// Struct
//

bool ImGui::BeginStruct(char const* lbl)
{
    ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
    ImGuiTreeNodeFlags tree_flags = ImGuiTreeNodeFlags_DefaultOpen;
    return ImGui::TreeNodeEx(lbl, tree_flags) && ImGui::BeginTable(lbl, 2, table_flags);
}

void ImGui::EndStruct()
{
    ImGui::EndTable();
    ImGui::TreePop();
}

void ImGui::StructField(std::string_view name, strparam value)
{
    TableNextColumn();
    TextUnformatted(name.data(), name.data() + name.size());

    TableNextColumn();
    TextUnformatted(value.data(), value.data() + value.size());
}

//
// Dockspace
//

static rectf g_dockspace_viewport;

rectf ImGui::GetDockspaceViewport()
{
    return g_dockspace_viewport;
}

void ImGui::RenderDockspace()
{

    auto viewport = ImGui::GetMainViewport();
    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiWindowClass const* window_class = nullptr;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |=
        ImGuiWindowFlags_NoTitleBar             |
        ImGuiWindowFlags_NoCollapse             |
        ImGuiWindowFlags_NoResize               |
        ImGuiWindowFlags_NoMove                 |
        ImGuiWindowFlags_NoDocking              |
        ImGuiWindowFlags_NoBringToFrontOnFocus  |
        ImGuiWindowFlags_NoNavFocus             |
        ImGuiWindowFlags_NoInputs;

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        host_window_flags |= ImGuiWindowFlags_NoBackground;

    auto label = PRINTER("DockSpaceViewport_%d", viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin(label.c_str(), NULL, host_window_flags);
    ImGui::PopStyleVar(3);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);

    auto node = ImGui::DockBuilderGetCentralNode(dockspace_id);
    if (node != nullptr)
    {
        g_dockspace_viewport.pos(node->Pos.x, node->Pos.y);
        g_dockspace_viewport.size(node->Size.x, node->Size.y);
    }

    ImGui::End();
}

//
// Misc. Functions
//

ImU32 ToU32(color::normal nor) { return ImGui::ColorConvertFloat4ToU32({nor.r, nor.g, nor.b, nor.a}); }
ImU32 ToU32(color::hsv hsv) { return ToU32(hsv.toNormal()); }
ImU32 ToU32(color col) { return ToU32(col.toNormal()); }

void ImGui::DrawDebugRectangle(cstrparam lbl, rectf r, color col,
    ImGuiDebugRectangleStyle style, cstrparam format)
{
    ImGuiContext&   g = *GImGui;
    ImGuiStyle&     s = ImGui::GetStyle();

    color::hsv hsv = col.toHSV();

    ImU32 c_border  = ToU32(hsv.opaque().withV(0.85f));          //GetColorU32(ImGuiCol_Border);
    ImU32 c_text    = ToU32(hsv.opaque().withV(1));              //GetColorU32(ImGuiCol_Text);
    ImU32 c_bg      = ToU32(hsv.withV(0.5f));                    //GetColorU32(ImGuiCol_TableHeaderBg);

    strview sv
    {
        g.TempBuffer,
        g.TempBuffer + ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer),
            format.c_str(), lbl.c_str(), r.min.x, r.min.y, r.width(), r.height())
    };

    auto [txt_w, txt_h] = ImGui::CalcTextSize(sv.begin(), sv.end());

    txt_w += s.CellPadding.x*2;
    txt_h += s.CellPadding.y*2;

    auto dl = ImGui::GetForegroundDrawList();
    r = r.round();

    switch (style)
    {
        case ImGuiDebugRectangleStyle_Default:
            dl->AddRect({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_border);

            r.size(txt_w, txt_h);
            dl->AddRectFilled({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_bg);
            dl->AddRect({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_border);

            dl->AddText({r.min.x+s.CellPadding.x, r.min.y+s.CellPadding.y}, c_text, sv.begin(), sv.end());
            break;

        case ImGuiDebugRectangleStyle_Full:
            dl->AddRect({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_border);
            dl->AddRectFilled({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_bg);
            dl->AddRect({r.min.x, r.min.y}, {r.max.x, r.max.y}, c_border);

            dl->AddText({r.min.x+s.CellPadding.x, r.min.y+s.CellPadding.y}, c_text, sv.begin(), sv.end());
            break;
    }
}

void ImGui::PushItemDisabled()
{
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

void ImGui::PopItemDisabled()
{
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}
