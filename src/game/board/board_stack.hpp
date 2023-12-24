//
// Created by james on 9/20/23.
//

#pragma once

#include "game/card_tank.hpp"

//
// gfx
//
#include "gfx/gfx_spinner.hpp"

namespace game::board
{
    class BoardStack
    {
    public:
        enum Type { DECK, YARD };

        BoardStack(Type type);

        inline Type type() const { return m_type; }
        inline bool isDeck() const { return m_type == DECK; }
        inline bool isYard() const { return m_type == YARD; }

        inline bool empty() const { return m_card_ngs.empty(); }
        inline size_t count() const { return m_card_ngs.size(); }

        void pushCard(CardRef ref);
        CardRef popCard();

        ng::Card popNgCard();
        void pushNgCard(ng::Card const& card);
        cardrefs_t getUnitCards(CardLayer layer);

        void setTestCards(size_t n);
        void clearTestCards() { setTestCards(0); }

        void setCards(ng::cardlist_t cards);

        void layout(ut::rect const& b);
        void update(float dt);

        void drawUnderCards();
        void drawAboveCards();
        void drawDebug();



    private:
        Type            m_type;

        ut::rect        m_bounds;
        gfx::Spinner    m_spinner;
        Texture2D       m_card_back;

        ng::cardlist_t  m_card_ngs;
    };
}