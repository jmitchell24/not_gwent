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
        box_ptr             selected_box;

        vt_type             view_transform;

        boxmap_t const& boxMap() const;
        ut::rect getRealRect(ut::rect const& r) const;
        ut::vec2 getViewPoint(ut::vec2 const& p) const;

        box_ptr     getBox      (ut::cstrparam s);
        void        setBox      (box_ptr const& ptr);
        void        clearBox    (box_ptr const& ptr);

    protected:
        boxmap_t m_box_map;

        void clearBoxMap();
        void setBoxMap(box_ptr const& ptr);
    };
}