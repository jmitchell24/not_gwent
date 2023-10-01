#include "imgui_mods.hpp"
#include "rlImGui/imgui/imgui.h"
#include "rlImGui/imgui/imgui_internal.h"

#include <ut/color.hpp>
using namespace ut;

#include "assert.hpp"

#define text_   ut::cstrparam
#define rect_   ut::rectf const&
#define color_  ut::color const&

//
// Struct
//

static bool in_struct = false;

bool ImGui::BeginStruct(text_ lbl)
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

void ImGui::StructField(text_ name, text_ value)
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

ImU32 ToU32(color::normal   const& nor  ) { return ImGui::ColorConvertFloat4ToU32({nor.r, nor.g, nor.b, nor.a}); }
ImU32 ToU32(color::hsv      const& hsv  ) { return ToU32(hsv.toNormal()); }
ImU32 ToU32(color::hsluv    const& hsluv) { return ToU32(hsluv.toNormal()); }
ImU32 ToU32(color           const& col  ) { return ToU32(col.toNormal()); }

void ImGui::DrawDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style )
{
    //ImGuiContext&   g = *GImGui;
    ImGuiStyle&     s = ImGui::GetStyle();

    auto hsluv      = col.toHSLUV();
    ImU32 c_border  = ToU32(hsluv.opaque().withL(85.0f));
    ImU32 c_text    = ToU32(hsluv.opaque().withL(100.0f));
    ImU32 c_bg      = ToU32(hsluv.withA(0.5f).withL(50.0f));

    cstrview sv;
    switch (style)
    {
        case ImGuiDRECTStyle_NoText:
            sv = ""_sv;
            break;
        case ImGuiDRECTStyle_LabelOnly:
            sv = lbl;
            break;
        case ImGuiDRECTStyle_ValueOnly:
            sv = PRINTER("%.0f,%.0f [%.0fx%.0f]", r.x(), r.y(), r.width(), r.height());
            break;
        case ImGuiDRECTStyle_FullText:
            sv = PRINTER("%s - %.0f,%.0f [%.0fx%.0f]", lbl.c_str(), r.x(), r.y(), r.width(), r.height());
            break;
        default:assert_case(ImGuiDRECTStyle_);
    }

    auto dl = ImGui::GetForegroundDrawList();
    auto rr = r.round();

    auto has_fill   = col.a > 0;
    auto has_text   = style != ImGuiDRECTStyle_NoText;

    if (has_fill)
    {
        dl->AddRect         (rr.min, rr.max, c_border);
        dl->AddRectFilled   (rr.min, rr.max, c_bg);
        dl->AddRect         (rr.min, rr.max, c_border);

        if (has_text)
        {
            dl->AddText(r.min + s.CellPadding, c_text, sv.begin(), sv.end());
        }
    }
    else
    {
        dl->AddRect(rr.min, rr.max, c_border);

        if (has_text)
        {
            auto txt_sz = ( vec2f(s.CellPadding) * 2 ) + ImGui::CalcTextSize(sv.begin(), sv.end());

            rr.size(txt_sz);
            dl->AddRectFilled(rr.min, rr.max, c_bg);
            dl->AddRect(rr.min, rr.max, c_border);
            dl->AddText(r.min + s.CellPadding, c_text, sv.begin(), sv.end());
        }
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

#undef text_
#undef rect_
#undef color_