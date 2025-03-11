//
// Created by james on 9/15/23.
//

#pragma once

#include "context.hpp"

#include "game/game_params.hpp"


namespace game
{

    /*
     Layers:
        - under board cards
        - above board cards
        - under overlay cards
        - above overlay cards

     */

    struct GameBoard2 : public GameParams
    {
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