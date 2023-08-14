#pragma once

#include "ng/cards.hpp"
#include "assert.hpp"

#include <cstddef>

namespace ng::gs
{
    struct CombatRow
    {
        static constexpr size_t ROW_SIZE = 20;
        Card cards[ROW_SIZE];

        unsigned score() const
        {
            assert_impl();
            return 0;
        }
    };

    struct Player
    {
        CombatRow melee, ranged, seige;

        inline unsigned score() const
        {
            return
                melee.score() +
                ranged.score() +
                seige.score();
        }
    };

    struct Game
    {
        Player p1, p2;
    };
}
