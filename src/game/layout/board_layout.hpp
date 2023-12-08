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
        ut::rect buff;
        ut::rect nerf;
        ut::rect units;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct PlayerRow
    {
        ut::rect bounds;
        ut::rect hand;
        ut::rect deck;
        ut::rect yard;
        ut::rect lead;

        void layout(ut::rect const& b);
        void drawDebug();
    };

//    struct WeatherBoard
//    {
//        ut::rect bounds;
//
//        void layout(ut::rect const& b);
//        void drawDebug();
//    };

    struct StatsBoard
    {
        ut::rect bounds;

        ut::rect name;
        ut::rect deck_name;
        ut::rect lead_name;
        ut::rect gems;
        ut::rect score;
        ut::rect avatar;

        void layout(ut::rect const& b);
        void drawDebug();
    };

    struct GameBoard
    {
        struct Player
        {
            ut::rect bounds;

            CombatRow   siege;
            CombatRow   ranged;
            CombatRow   melee;
            PlayerRow   player;
            StatsBoard  stats;
        };

        ut::rect bounds;

        Player usr, cpu;

        void layout(ut::rect const& b);
        void drawDebug();

    };

    struct CardPicker
    {
        ut::rect bounds;

        ut::rect title;
        ut::rect cards;
        ut::rect confirm;
        ut::rect cancel;
    };
}
