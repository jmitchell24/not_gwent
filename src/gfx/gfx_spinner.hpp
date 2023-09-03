//
// Created by james on 9/3/23.
//

#pragma once

#include "gfx/gfx_tween.hpp"

namespace gfx
{
    class Spinner
    {
    public:
        inline int value() const { return m_value; }

        void set(int value);
        void anim(int value);
        void layout(ut::rectf const& bounds);
        void update(float dt);
        void draw();

    private:
        int             m_value;
        TweenReal       m_tween_target{easings::elasticOut, 2.0f};

        ut::rectf       m_bounds;
        ut::rectf       m_bounds_a;
        ut::rectf       m_bounds_b;

        int             m_integer_a;
        int             m_integer_b;

        ut::color       m_color_a;
        ut::color       m_color_b;

        void updateValues(float f);
    };

}