//
// Created by james on 9/23/23.
//

#pragma once

#include "game/card.hpp"

namespace game::board
{
    class BoardElement
    {
    public:
        virtual bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
    private:
    };
}