#pragma once

#include "assert.hpp"
#include "ng/ng_card_data.hpp"

namespace ng
{
    struct CombatRow
    {
        Card        special;
        cardlist_t  cards;
    };

    struct Player
    {
        unsigned round_wins = 0;

        Card        leader;
        cardlist_t  hand;
        cardlist_t  deck;
        cardlist_t  graveyard;
        cardlist_t  weather;
        CombatRow   combat_row_melee;
        CombatRow   combat_row_ranged;
        CombatRow   combat_row_siege;

        inline unsigned strength() const
        {
            unsigned strength=0;

            for (auto&& it : combat_row_melee.cards)
                if (it.isUnitCard())
                    strength += it.asUnitCard().strength;

            for (auto&& it : combat_row_ranged.cards)
                if (it.isUnitCard())
                    strength += it.asUnitCard().strength;

            for (auto&& it : combat_row_siege.cards)
                if (it.isUnitCard())
                    strength += it.asUnitCard().strength;

            return strength;
        }
    };

    struct Game
    {
        Player p1, p2;
    };
}
