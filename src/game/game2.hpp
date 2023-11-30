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

        //
        //
        //

        bool    tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
        //void    playCard(Player& player, size_t idx);
        int     updatePlayerScore(Player& player);
        void    updateScores(Player& player);

        //void cast(ut::vec2 const& mp);


    };

    struct DoCastingThing
    {
        GameBoard2& gb;
        void operator() (CastUnit const& c);
        void operator() (CastSpy const& c);
        void operator() (CastRowBuff const& c);
        void operator() (CastRowNerf const& c);
        void operator() (CastScorch const& c);
        void operator() (CastLeaderAbility const& c);
        void operator() (CastWeather const& c);
    };

}