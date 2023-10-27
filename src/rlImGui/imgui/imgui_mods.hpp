
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

#define text_       ut::cstrparam
#define rect_       ut::rectf const&
#define color_      ut::color const&
#define frame_      Frame const&

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
    // ColorEdit
    //

    inline bool ColorEdit4(char const* label, ut::color& col, ImGuiColorEditFlags flags = 0)
    {
        if (auto v = col.toNormal().toVec4(); ColorEdit4(label, v.pack, flags))
        {
            col = ut::color{ut::color::normal{v}};
            return true;
        }
        return false;
    }

    //
    // Extra Buttons
    //

    struct ButtonColor { ut::color b,h,a,t; };

    inline ButtonColor CreateButtonColor(ut::color::hsluv const& c)
    {
        return
        {
            {ut::color::hsluv{c.h, c.s, 66.0f, 0.4f}.toColor()},
            {ut::color::hsluv{c.h, c.s, 66.0f, 1.0f}.toColor()},
            {ut::color::hsluv{c.h, c.s, 50.0f, 1.0f}.toColor()},
            {ut::colors::white}
        };
    }

    inline ButtonColor CreateActivatedButtonColor(ut::color::hsluv const& c)
    {
        return
        {
            {ut::color::hsluv{c.h, c.s, 75.0f, 1.0f}.toColor()},
            {ut::color::hsluv{c.h, c.s, 85.0f, 1.0f}.toColor()},
            {ut::colors::white},
            {ut::colors::black}
        };
    }

    inline void PushButtonColor(ButtonColor const& c)
    {
        PushStyleColor(ImGuiCol_Button          , c.b);
        PushStyleColor(ImGuiCol_ButtonHovered   , c.h);
        PushStyleColor(ImGuiCol_ButtonActive    , c.a);
        PushStyleColor(ImGuiCol_Text            , c.t);
    }

    inline void PushButtonColor(color_ c)
    { PushButtonColor(CreateButtonColor(c.toHSLUV())); }

    inline void PushActivatedButtonColor()
    { PushButtonColor(CreateActivatedButtonColor(ut::colors::hsluv::goldenrod())); }

    inline void PopButtonColor()
    {
        PopStyleColor(4);
    }

    void PushItemDisabled();
    void PopItemDisabled();

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

    inline bool ButtonActivated(text_ lbl, bool is_activated)
    {
        if (is_activated)
        {
            PushActivatedButtonColor();
            Button(lbl);
            PopButtonColor();
            return false;
        }

        return Button(lbl);
    }

    inline bool SmallButtonActivated(text_ lbl, bool is_activated)
    {
        if (is_activated)
        {
            PushActivatedButtonColor();
            SmallButton(lbl);
            PopButtonColor();
            return false;
        }

        return SmallButton(lbl);
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

    inline bool SmallActivatedButtonConfirm(text_ lbl, bool enabled, text_ prompt = "Are you sure?", text_ lbl_confirm = "yes")
    {
        if (SmallButtonActivated(lbl, enabled))
        {
            OpenPopup("SmallActivatedButtonConfirm");
        }

        if (BeginPopup("SmallActivatedButtonConfirm"))
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

    inline bool ButtonDefault(char const* lbl, bool enabled)
    {
        using namespace ImGui;

        bool clicked = ButtonEnabled(ut::PRINTER("D##%s", lbl), enabled);
        if (IsItemHovered())
            SetTooltip("Set to Default");
        SameLine();
        return clicked;
    }

    //
    // Misc. Functions
    //

    inline ImU32 ToU32(ut::color::normal   const& nor  ) { return ImGui::ColorConvertFloat4ToU32({nor.r, nor.g, nor.b, nor.a}); }
    inline ImU32 ToU32(ut::color::hsv      const& hsv  ) { return ToU32(hsv.toNormal()); }
    inline ImU32 ToU32(ut::color::hsluv    const& hsluv) { return ToU32(hsluv.toNormal()); }
    inline ImU32 ToU32(ut::color           const& col  ) { return ToU32(col.toNormal()); }

    inline void TextUnformatted(text_ t)
    {
        TextUnformatted(t.begin(), t.end());
    }

    struct Frame
    {
        ut::rect outer, inner;
        struct Colors
        {
            ImU32 bd, bg;
        } cols;

        Frame(rect_ o, rect_ i, color_ c)
        {
            outer = o;
            inner = i;

            //auto h = c.opaque().toHSLUV();
            cols.bd   = ToU32(ut::colors::red);
            cols.bg   = ToU32(ut::colors::green);

        }
    };



    void DrawDRECT(ImDrawList* dl, text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style);

    inline void DrawDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetForegroundDrawList(), lbl, r, col, style); }

    inline void DrawForegroundDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetForegroundDrawList(), lbl, r, col, style); }

    inline void DrawBackgroundDRECT(text_ lbl, rect_ r, color_ col, ImGuiDRECTStyle style = ImGuiDRECTStyle_NoText)
    { DrawDRECT(GetBackgroundDrawList(), lbl, r, col, style); }

    void DrawDFRAME(ImDrawList* dl, frame_ f);

    inline void DrawBackgroundDFRAME(frame_ f)
    { DrawDFRAME(GetBackgroundDrawList(), f); }

    inline void DrawForegroundDFRAME(frame_ f)
    { DrawDFRAME(GetForegroundDrawList(), f); }

}

#undef text_
#undef rect_
#undef color_
#undef frame_