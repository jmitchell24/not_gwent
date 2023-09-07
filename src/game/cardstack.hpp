//
// Created by james on 9/6/23.
//

#pragma once

#include "game/card.hpp"

namespace game
{
    class CardStack
    {
    public:
        CardStack();
        CardStack(cardlist_t cards);

        inline bool empty() { return m_cards.empty(); }
        inline size_t size() const { return m_cards.size(); }

        inline ut::rect const& bounds() const { return m_bounds; }
        inline ut::rect const& boundsCards() const { return m_bounds_cards; }

        void setCards(cardlist_t cards);
        void push(Card const& card);
        Card pop();

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        ut::rect m_bounds;
        ut::rect m_bounds_cards;
        cardlist_t m_cards;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}