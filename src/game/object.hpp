//
// Created by james on 8/5/23.
//

#pragma once

#include "assert.hpp"

#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

#include <memory>

namespace game
{
//    class Object2D
//    {
//    public:
//        bool loaded() const { return m_loaded; }
//
//        void load();
//        void unload();
//
//        void layout();
//        void update();
//        void draw();
//    protected:
//
//    private:
//        bool m_loaded = false;
//
//    };

    class Layout
    {
    public:
        bool isLayoutReady() const { return m_layout_ready; }
        ut::rect const& bounds() const { assert(m_layout_ready); return m_bounds; }

        void layout(ut::rect const& bounds)
        {
            m_layout_ready = true;
            m_bounds = bounds;
            onLayout(bounds);
        }

    protected:
        bool m_layout_ready=false;
        ut::rect m_bounds;

        virtual void onLayout(ut::rect const& bounds)
        { m_bounds = bounds; }
    };
}