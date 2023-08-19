
#pragma once

#include "rlImGui/imgui/imgui.h"

#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>


#define STRUCT_BEGIN(__x__) if (ImGui::BeginStruct(#__x__)) { auto const& __y__ = __x__;
#define STRUCT_FIELD(__x__) ImGui::StructFieldFmt(#__x__, "{}", __y__.__x__);
#define STRUCT_END          ImGui::EndStruct(); }

//#define IM_DEBUG_RECTANGLE_FORMAT "\"%s\" %.0f,%.0f [%.0fx%.0f]"
#define IM_DEBUG_RECTANGLE_FORMAT "%s"

//
// enums
//

enum ImGuiDebugRectangleStyle_
{
    ImGuiDebugRectangleStyle_Default,
    ImGuiDebugRectangleStyle_Full,
    ImGuiDebugRectangleStyle_Simple
};

using ImGuiDebugRectangleStyle = int;

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



    void DrawDebugRectangle(ut::cstrparam lbl, ut::rectf r, ut::color col,
        ImGuiDebugRectangleStyle style = ImGuiDebugRectangleStyle_Full,
        ut::cstrparam format = IM_DEBUG_RECTANGLE_FORMAT);


}
