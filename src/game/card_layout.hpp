//
// Created by james on 9/9/23.
//

#pragma once

#include "game/card_container.hpp"

namespace game
{
    struct CardLayout
    {
        enum Direction { DIR_LEFT, DIR_CENTER };

        ut::rect    bounds;

        size_t      card_count;
        float       card_width;
        float       card_gap;

        static CardLayout create(Direction direction, ut::rect const& bounds, float card_width, size_t card_count);
        bool tryGetIndex(ut::vec2 const& mp, size_t& idx) const;

        [[nodiscard]] ut::vec2 getPos(size_t idx) const;
        [[nodiscard]] ut::rect getRect(size_t idx) const;
    };
}