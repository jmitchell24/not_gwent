//
// Created by james on 9/1/23.
//

#pragma once

#include "ng/ng_action.hpp"
#include "ng/ng_game.hpp"

namespace ng
{
    class GameSolver
    {
    public:

        bool applyAction(Action const& action, gamechangelist_t& changes);
        static Game makeGame();
    private:
    };
}