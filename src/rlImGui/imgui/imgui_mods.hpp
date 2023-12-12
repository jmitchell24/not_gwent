
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
#define wcolors_    WidgetColors const&
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

    inline bool ColorEdit3(char const* label, ut::color& col, ImGuiColorEditFlags flags = 0)
    {
        if (auto v = col.toNormal().toVec4(); ColorEdit3(label, v.pack, flags))
        {
            col = ut::color{ut::color::normal{v}};
            return true;
        }
        return false;
    }

    //
    // Extra Buttons
    //

    struct WidgetColors
    {
        ut::color color;
        ut::color color_hovered;
        ut::color color_active;
        ut::color color_text;

        static WidgetColors fromHSLUV(ut::color::hsluv const& c);
        static WidgetColors fromHSLUVAlt(ut::color::hsluv const& c);
        static WidgetColors fromStyle(ImGuiCol c, ImGuiCol ch, ImGuiCol ca, ImGuiCol ct);

        void pushButtonColor() const;
        void pushTabColor() const;
        void pushHeaderColor() const;
        void popColor() const;
    };

    extern WidgetColors const WC_YELLOW;
    extern WidgetColors const WC_ORANGE;
    extern WidgetColors const WC_RED;
    extern WidgetColors const WC_VIOLET;
    extern WidgetColors const WC_BLUE;
    extern WidgetColors const WC_GREEN;

    static WidgetColors const& WC_HIGHLIGHT = WC_ORANGE;

    void PushItemDisabled();
    void PopItemDisabled();

    inline bool Button(text_ lbl, wcolors_ wc)
    {
        wc.pushButtonColor();
        bool b = Button(lbl);
        wc.popColor();
        return b;
    }

    inline bool SmallButton(text_ lbl, wcolors_ wc)
    {
        wc.pushButtonColor();
        bool b = SmallButton(lbl);
        wc.popColor();
        return b;
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

    inline bool SmallButtonEnabled(text_ lbl, bool is_enabled, wcolors_ wc)
    {
        if (is_enabled)
            return SmallButton(lbl, wc);
        PushItemDisabled();
        SmallButton(lbl, wc);
        PopItemDisabled();
        return false;
    }

    inline bool ButtonActivated(text_ lbl, bool is_activated, wcolors_ wc_activated = WC_HIGHLIGHT)
    {
        if (is_activated)
        {
            Button(lbl, wc_activated);
            return false;
        }

        return Button(lbl);
    }

    inline bool SmallButtonActivated(text_ lbl, bool is_activated, wcolors_ wc_activated = WC_HIGHLIGHT)
    {
        if (is_activated)
        {
            SmallButton(lbl, wc_activated);
            return false;
        }

        return SmallButton(lbl);
    }

    inline bool ButtonSelected(text_ lbl, bool is_selected, wcolors_ wc_selected = WC_HIGHLIGHT)
    {
        if (is_selected)
            return Button(lbl, wc_selected);
        return Button(lbl);
    }

    inline bool SmallButtonSelected(text_ lbl, bool is_selected, wcolors_ wc_selected = WC_HIGHLIGHT)
    {
        if (is_selected)
            return SmallButton(lbl, wc_selected);
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

    inline bool CollapsingHeaderSelected(char const* lbl, bool is_selected, ImGuiTreeNodeFlags flags=0, wcolors_ wc_selected = WC_HIGHLIGHT)
    {
        if (is_selected)
        {
            wc_selected.pushHeaderColor();
            bool b = CollapsingHeader(lbl, flags);
            wc_selected.popColor();
            return b;
        }
        return CollapsingHeader(lbl, flags);
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