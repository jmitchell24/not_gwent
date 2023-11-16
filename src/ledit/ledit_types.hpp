//
// Created by james on 10/29/23.
//

#pragma once

//
// ut
//
#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

//
// std
//
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include <ut/check.hpp>

namespace ledit
{
    class Box;
    using box_ptr   = std::shared_ptr<Box>;
    using box_cptr  = std::shared_ptr<Box const>;
    using boxmap_t  = std::unordered_map<std::string, box_ptr>;
    using boxlist_t = std::vector<box_ptr>;

    using rectget_t  = std::optional<ut::rect>;

    struct BoxEditOptions
    {
        static constexpr ut::cstrview SELECTED_BOX_POPUP_LBL = "selected_box_popup";

        bool show_row_select    = true;
        bool show_row_add       = false;
        bool show_row_delete    = false;
        bool show_row_move      = false;
        bool show_row_rename    = false;
        bool show_row_weight    = false;
        bool show_row_type      = false;

        bool is_properties_window_open = false;
    };

    struct OverlayOptions
    {
        struct Style { ut::color background, border; };

        static constexpr Style STYLE_PRESET_DEFAULT {  {15,15,15,240}, {110,110,128,128} };
        static constexpr Style STYLE_PRESET_OPAQUE  {  {15,15,15,255}, {110,110,128,128} };
        static constexpr Style STYLE_PRESET_FADE    {  {15,15,15,64 }, {255,255,255,255} };
        static constexpr Style STYLE_PRESET_HIDE    {  { 0, 0, 0,  0}, {  0,  0,  0,  0} };

        std::array<Style, 4> styles
        {
            STYLE_PRESET_DEFAULT,
            STYLE_PRESET_OPAQUE,
            STYLE_PRESET_FADE,
            STYLE_PRESET_HIDE
        };

        int         style_index         =0;
        bool        want_capture_mouse  =false;

        inline Style& style()
        {
            return styles[style_index%styles.size()];
        }
    };
}