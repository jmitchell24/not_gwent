
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

    inline void PushButtonColor(color_ c)
    {
        auto h  = c.toHSLUV();
        auto b  = ut::color::hsluv{h.h, h.s, 66.0f, 0.4f};
        auto bh = b.withA(1.0f);
        auto ba = h.withL(50.0f);

        PushStyleColor(ImGuiCol_Button, b);
        PushStyleColor(ImGuiCol_ButtonHovered, bh);
        PushStyleColor(ImGuiCol_ButtonActive, ba);
    }

    inline void PopButtonColor()
    {
        PopStyleColor(3);
    }

    inline bool ButtonEnabled(text_ lbl, bool is_enabled)
    {
        if (is_enabled)
            return Button(lbl);
        PushItemDisabled();
        Button(lbl);
        PopItemDisabled();
        return false;
    }

    inline bool SmallButtonEnabled(text_ lbl, bool is_enabled)
    {
        if (is_enabled)
            return SmallButton(lbl);
        PushItemDisabled();
        SmallButton(lbl);
        PopItemDisabled();
        return false;
    }

    inline bool ButtonConfirm(text_ lbl, text_ prompt = "Are you sure?", text_ lbl_confirm = "yes")
    {
        if (Button(lbl))
        {
            OpenPopup("ButtonConfirm");
        }

        if (BeginPopup("ButtonConfirm"))
        {
            bool b = SmallButton(lbl_confirm);
            if (b) CloseCurrentPopup();
            SameLine();
            TextUnformatted(prompt.begin(), prompt.end());
            EndPopup();
            return b;
        }

        return false;
    }

    inline bool SmallButtonConfirm(text_ lbl, text_ prompt = "Are you sure?", text_ lbl_confirm = "yes")
    {
        if (SmallButton(lbl))
        {
            OpenPopup("SmallButtonConfirm");
        }

        if (BeginPopup("SmallButtonConfirm"))
        {
            bool b = SmallButton(lbl_confirm);
            if (b) CloseCurrentPopup();
            SameLine();
            TextUnformatted(prompt.begin(), prompt.end());
            EndPopup();
            return b;
        }

        return false;
    }


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