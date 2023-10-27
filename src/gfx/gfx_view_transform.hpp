//
// Created by james on 10/27/23.
//

#pragma once

#include <ut/math.hpp>


namespace gfx
{
    struct ViewTransform
    {
    public:
        ut::mat4 translate;
        ut::mat4 translate_inverse;

        [[nodiscard]] ut::vec2 realPoint(ut::vec2 const& p) const;
        [[nodiscard]] ut::rect realRect (ut::rect const& r) const;
        [[nodiscard]] ut::vec2 viewPoint(ut::vec2 const& p) const;
        [[nodiscard]] ut::rect viewRect (ut::rect const& r) const;

        static ViewTransform create(ut::rect const& viewport, float width, float height);
    };
}