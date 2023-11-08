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
        static constexpr ut::color DEFAULT_BACKGROUND   {15 ,15 ,15 ,240};
        static constexpr ut::color DEFAULT_BORDER       {110,110,128,128};

        ut::color   background =DEFAULT_BACKGROUND;
        ut::color   border     =DEFAULT_BORDER;

        bool ignore_mouse=false;
    };
}