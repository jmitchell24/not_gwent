//
// Created by james on 3/4/24.
//

#pragma once

#include "game/board/board_boss.hpp"
#include "game/game_card_picker.hpp"
#include "game/game_player.hpp"
#include "game/game_move.hpp"
#include "game/game_io.hpp"

//TODO: come up with a better name...

namespace game
{
    struct GameParams
    {
        Player              usr, cpu;
        CardPicker          card_picker;
        CardRef             card_hover;
        board::BoardBoss    boss;

        game_layout::GameBoard gb;
    };
}