
#pragma once

#include "rlImGui/imgui/imgui.h"

#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>

//
// enums
//

enum ImGuiDRECTStyle_
{
    ImGuiDRECTStyle_NoText,
    ImGuiDRECTStyle_LabelOnly,
    ImGuiDRECTStyle_ValueOnly,
    ImGuiDRECTStyle_FullText
};

using ImGuiDRECTStyle = int;

#define text_    ut::cstrparam
#define rect_   ut::rectf const&
#define color_  ut::color const&

namespace ImGui
{
    //
    // Struct
    //

    bool BeginStruct(text_ lbl = {});
    void EndStruct();

    void StructField(text_ lbl, text_ value);

//    template <typename... T>
//    inline void StructFieldFmt(std::string_view name, fmt::format_string<T...> fmt, T&&... args)
//    {
//        auto value = fmt::format(fmt, args...);
//        StructField(name, std::string_view{value});
//    }



    //
    // Dockspace
    //

    ut::rectf GetDockspaceViewport();
    void RenderDockspace();

    //
    // Misc. Functions
    //

    void PushItemDisabled();
    void PopItemDisabled();


    void DrawDRECT(ImDrawList* dl, text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style);

    inline void DrawDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetForegroundDrawList(), lbl, r, col, style); }

    inline void DrawForegroundDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetForegroundDrawList(), lbl, r, col, style); }

    inline void DrawBackgroundDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetBackgroundDrawList(), lbl, r, col, style); }
}

#undef text_
#undef rect_
#undef color_