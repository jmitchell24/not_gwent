//
// Created by james on 9/20/23.
//

#pragma once

#include "game/card_tank.hpp"

namespace game::board
{
    class BoardSlot
    {
    public:
        inline bool isEmpty() { return !m_card; }
        inline CardRef card() const { return m_card; }

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);

        void takeCard(CardRef ref);
        CardRef giveCard();
        void layout(ut::rect const& b);

        void drawAboveCards();
        void drawUnderCards();
        void drawDebug     ();
    private:
        ut::rect    m_bounds;
        ut::rect    m_bounds_card;
        CardRef     m_card;
    };
}