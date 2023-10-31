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
        BoxEditOptions      edit_opts;
        OverlayOptions      overlay_opts;
        box_ptr             selected_box;

        std::optional<gfx::ViewTransform> view_transform;

        ut::rect getRealRect(ut::rect const& r) const;
        ut::vec2 getViewPoint(ut::vec2 const& p) const;


        box_ptr     getBox      (ut::cstrparam s);
        void        setBox      (box_ptr const& ptr);
        void        clearBox    (box_ptr const& ptr);

        inline boxmap_t const& boxmap() const { return m_boxmap; }

    protected:
        boxmap_t m_boxmap;
    };
}