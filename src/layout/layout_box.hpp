//
// Created by james on 12/14/23.
//
#pragma once

#include <ut/math.hpp>

namespace layout
{
    struct LayoutBox
    {
        ut::rect outer;
        ut::rect border;
        ut::rect inner;

        inline LayoutBox()=default;

        inline LayoutBox(ut::rect const& r)
        : outer{r}, border{r}, inner{r} {}

        inline operator ut::rect() const
        { return border; }
    };
}