//
// Created by james on 9/22/23.
//

#pragma once

#include "game/board/board_slot.hpp"
#include "game/board/board_row.hpp"
#include "game/board/board_stack.hpp"

namespace game::board
{
    class BoardBoss
    {
    public:
        void slotToRow  (BoardSlot& slot, BoardRow& row, size_t idx);
        void rowToSlot  (BoardRow& row, BoardSlot& slot, size_t idx);
        void rowToStack (BoardRow& row, BoardStack& stack, size_t idx);
        void stackToRow (BoardStack& stack, BoardRow& row, size_t idx);
        void rowToRow   (BoardRow& row_a, BoardRow& row_b, size_t idx_a, size_t idx_b);

        void update(float dt);

    private:
        cardrefs_t m_terminal_cards;
    };
}