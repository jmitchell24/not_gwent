//
// Created by james on 9/24/23.
//

#pragma once

#include "game/board/board_slot.hpp"

namespace board
{
    class BoardStats
    {
    public:
        void layout (ut::rect const& b);
        void update (float dt);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug     ();

    private:
    };
}