//
// Created by james on 9/9/23.
//

#pragma once

#include "game/layout/card_layout.hpp"

#include <ut/math.hpp>

namespace game
{
    struct RowLayout
    {
        ut::rect    bounds;

        size_t      card_count;
        float       card_width;
        float       card_gap;

        static RowLayout create(ut::rect const& bounds, float card_width, size_t card_count);
        bool tryGetIndex(ut::vec2 const& mp, size_t& idx) const;

        ut::vec2 getPos(size_t idx) const;
        ut::rect getRect(size_t idx) const;

        CardLayout getCardLayout(size_t idx) const;

        //void drawDebug(){}
    };
}