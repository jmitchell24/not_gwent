//
// Created by james on 9/15/23.
//

#pragma once

#include "context.hpp"

#include "game/board/board_boss.hpp"
#include "game/game_player.hpp"
#include "game/game_io.hpp"


namespace game
{


    struct GameBoard2
    {
        Player usr, cpu;

        CardRef card_hover;

        layout::GameBoard gb;

        board::BoardBoss boss;

        GameBoard2();

        void layout(ut::rect const& bounds);
        void update(update_param u);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug     ();
        void drawDebugMenu ();

        bool    tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
        int     updatePlayerScore(Player& player);
        void    updateScores(Player& player);
    };



}