//
// Created by james on 9/15/23.
//

#pragma once

#include "context.hpp"

#include "game/board/board_boss.hpp"
#include "game/game_card_picker.hpp"
#include "game/game_player.hpp"
#include "game/game_io.hpp"


namespace game
{

    /*
     Layers:
        - under board cards
        - above board cards
        - under overlay cards
        - above overlay cards

     */

    struct GameBoard2
    {
        Player              usr, cpu;
        CardPicker          card_picker;
        CardRef             card_hover;
        board::BoardBoss    boss;

        game_layout::GameBoard gb;


        GameBoard2();

        void layout(ut::rect const& bounds);
        void update(update_param u);

        void drawUnderBoardCards();
        void drawAboveBoardCards();
        void drawUnderOverlayCards();
        void drawAboveOverlayCards();

        void drawDebug     ();
        void drawDebugMenu ();

        bool    tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
        int     updatePlayerScore(Player& player);
        void    updateScores(Player& player);
    };



}