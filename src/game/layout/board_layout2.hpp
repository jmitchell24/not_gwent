//
// Created by james on 9/11/23.
//

#pragma once

#include "game/layout/board_layout.hpp"

namespace game::layout
{


    struct GraphicsLayout
    {
        struct Buttons
        {
            ut::rect bounds;

            ut::rect buff_outer;
            ut::rect buff_inner;

            ut::rect nerf_outer;
            ut::rect nerf_inner;

            ut::rect score_outer;
            ut::rect score_inner;

            void layout(CombatRow const& row);

            void drawDebug();
        };

        ut::rect outer;
        ut::rect inner;

        ut::rect board_outer;
        ut::rect board_inner1;
        ut::rect board_inner2;

        ut::rect cards_outer;
        ut::rect cards_inner;

        Buttons cpu_siege_buttons;
        Buttons cpu_ranged_buttons;
        Buttons cpu_melee_buttons;

        Buttons usr_melee_buttons;
        Buttons usr_ranged_buttons;
        Buttons usr_siege_buttons;

        void layout(GameBoard const& gb);
        void drawDebug();
    };
}
