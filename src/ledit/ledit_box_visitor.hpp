//
// Created by james on 10/29/23.
//

#pragma once

#include "ledit/ledit_types.hpp"

//
// gfx
//
#include "gfx/gfx_view_transform.hpp"

namespace ledit
{
    class BoxVisitor
    {
    public:
        using vt_type = std::optional<gfx::ViewTransform>;

        BoxEditOptions      edit_opts;
        OverlayOptions      overlay_opts;

        box_ptr             root_box;
        box_ptr             selected_box_current;
        box_ptr             selected_box_previous;

        vt_type             view_transform;

        boxmap_t const& boxMap() const;
        ut::rect getRealRect(ut::rect const& r) const;
        ut::vec2 getViewPoint(ut::vec2 const& p) const;

        box_ptr     getBoxSlot      (ut::cstrparam s);
        void        setBoxSlot      (box_ptr const& ptr);
        void        resetBoxSlot    (box_ptr const& ptr);
        void        resetAllSlots   ();

        inline void setSelectedBox(box_ptr const& ptr)
        {
            if (selected_box_current != ptr)
            {
                selected_box_previous = selected_box_current;
                selected_box_current = ptr;
            }
        }

        inline void clearSelectedBox()
        {
            selected_box_current = nullptr;
            selected_box_previous = nullptr;
        }

        inline bool isSelected(box_ptr const& ptr) const
        {
            return selected_box_current == ptr;
        }

        inline bool isSelectedPrevious(box_ptr const& ptr) const
        {
            return selected_box_previous == ptr;
        }

        inline char const* selectedLbl(box_ptr const& ptr) const
        {
            if (selected_box_current == ptr)
                return "1";
            if (selected_box_previous == ptr)
                return "2";
            return " ";
        }

    protected:
        boxmap_t m_box_map;

        void setBoxSlotAll(box_ptr const& ptr);
    };
}