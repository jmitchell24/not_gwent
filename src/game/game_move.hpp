//
// Created by james on 12/24/23.
//

#pragma once

#include <variant>

#include "game/game_enums.hpp"
#include "game/board/board_row.hpp"
#include "game/board/board_slot.hpp"
#include "game/board/board_stack.hpp"

namespace game
{
    class Player;

    struct CardMoveNil;
    struct CardMoveRow;
    struct CardMoveStack;

    using CardMove = std::variant<
        CardMoveNil,
        CardMoveRow,
        CardMoveStack
    >;

    struct CardMoveNil { };

    struct CardMoveRow
    {
        enum RowType { MELEE, RANGED, SIEGE, HAND };
        RowType     row;
        size_t      idx;
        CardRef     ref;
    };

    struct CardMoveStack
    {
        enum StackType { DECK, YARD };
        StackType   stack;
        CardRef     ref;
    };

    struct CardMoveParams
    {
        Player& player;
    };
}

namespace game::visitors
{
    struct MoveCard
    {
        CardMoveParams params;

        void operator()(CardMoveNil const&);
        void operator()(CardMoveRow const& m);
        void operator()(CardMoveStack const& m);
    };
}