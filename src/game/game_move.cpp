//
// Created by james on 1/20/24.
//

#include "game/game_move.hpp"
#include "game/game_player.hpp"

using namespace game;
using namespace game::board;
using namespace game::visitors;

//
// std
//
using namespace std;

void MoveCard::operator()(CardMoveNil const& m)
{
    nopath_impl;
}

void MoveCard::operator()(CardMoveRow const& m)
{
    nopath_impl;
}

void MoveCard::operator()(CardMoveStack const& m)
{
    nopath_impl;
}