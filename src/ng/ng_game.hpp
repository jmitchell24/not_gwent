#pragma once

#include "ng/ng_card_data.hpp"
#include "assert.hpp"

#include "ut/container/stack_array.hpp"

#include <cstddef>

namespace ng::gs
{
    struct CombatRow
    {
        static constexpr size_t ROW_SIZE = 20;
        Card cards[ROW_SIZE];
        size_t count=0;

        unsigned score() const
        {
            unsigned score;
            for (size_t i = 0; i < count; ++i)
            {
                if (cards[i].isUnitCard())
                    score += cards[i].asUnitCard().strength;
            }
            return score;
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
