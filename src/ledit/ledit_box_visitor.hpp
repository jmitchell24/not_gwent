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

        bool is_properties_window_open  = false;
        bool is_overlay_visible         = false;
        bool want_capture_mouse         = false;
        bool show_row_select            = true;

        box_ptr root_box;
        vt_type view_transform;

        explicit BoxVisitor(ut::cstrparam name);

        std::string const& name() const;

        ut::rect getRealRect(ut::rect const& r) const;
        ut::vec2 getViewPoint(ut::vec2 const& p) const;
        std::string toCPPString();

        //
        // Box Slots
        //

        boxmap_t const& boxMap() const;
        size_t getSlotCount() const;
        size_t getFilledSlotCount() const;
        size_t getEmptySlotCount() const;
        box_ptr getBoxSlot(ut::cstrparam s);
        void setBoxSlot(box_ptr const& ptr);
        void resetBoxSlot(box_ptr const& ptr);
        void resetAllSlots();

        //
        // Box Selection
        //

        void setSelectedBoxSingle(box_ptr const& ptr);
        void setSelectedBoxMulti(box_ptr const& ptr);
        void toggleSelectedBoxMulti(box_ptr const& ptr);
        void setMutateSelection(box_ptr const& ptr);

        void clearSelectedBoxSingle();
        void clearSelectedBoxMulti(box_ptr const& ptr);
        void clearSelectedBoxMultiAll();

        bool isBoxSelectedSingle(box_ptr const& ptr) const;
        bool isBoxSelectedMulti(box_ptr const& ptR) const;
        bool hasBoxSelectionSingle() const;
        bool hasBoxSelectionMulti() const;

        /// allow mutate only on sibling boxes
        bool canMutate() const;

        box_ptr boxSelectionSingle() const;
        boxlist_t boxSelectionMulti() const;

        ut::cstrview getBoxSelectionLabel(box_ptr const& ptr) const;

    protected:
        std::string m_name;
        boxmap_t    m_box_map;

        void setBoxSlotAll(box_ptr const& ptr);

    private:
        box_ptr     m_selected_box_single;
        boxlist_t   m_selected_box_multi;
    };
}