
#pragma once

#include "rlImGui/imgui/imgui.h"

#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>


#define STRUCT_BEGIN(__x__) if (ImGui::BeginStruct(#__x__)) { auto const& __y__ = __x__;
#define STRUCT_FIELD(__x__) ImGui::StructFieldFmt(#__x__, "{}", __y__.__x__);
#define STRUCT_END          ImGui::EndStruct(); }

//#define IM_DEBUG_RECTANGLE_FORMAT "%s - %.0f,%.0f [%.0fx%.0f]"
//#define IM_DEBUG_RECTANGLE_FORMAT "%s"

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

#define lbl_    ut::cstrparam
#define rect_   ut::rectf const&
#define color_  ut::color const&

namespace ImGui
{
    //
    // Struct
    //

    bool BeginStruct(char const* lbl = nullptr);
    void EndStruct();

    void StructField(std::string_view name, ut::strparam value);

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


    void DrawDRECT(lbl_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText);
}

#undef lbl_
#undef rect_
#undef color_