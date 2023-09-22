//
// Created by james on 9/11/23.
//

#pragma once

#include <ut/math.hpp>

namespace game::layout
{
    struct CombatRow
    {
        ut::rect bounds;
        ut::rect score;
        ut::rect special;
        ut::rect units;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct PlayerRow
    {
        ut::rect bounds;
        ut::rect hand;
        ut::rect deck;
        ut::rect graveyard;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct WeatherBoard
    {
        ut::rect bounds;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct StatsBoard
    {
        ut::rect bounds;

        ut::rect name;
        ut::rect deck_name;
        ut::rect gems;
        ut::rect gems_label;
        ut::rect hand_count;
        ut::rect hand_count_label;
        ut::rect score;
        ut::rect score_label;
        ut::rect avatar;
        ut::rect leader;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct GameBoard
    {
        struct Player
        {
            CombatRow   siege;
            CombatRow   ranged;
            CombatRow   melee;
            PlayerRow   player;
            StatsBoard  stats;
        };

        ut::rect bounds;

        ut::vec2 card_size;

        Player usr, cpu;

        WeatherBoard weather;

        void layout(ut::rect const& b);
        void drawDebug();


#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}
