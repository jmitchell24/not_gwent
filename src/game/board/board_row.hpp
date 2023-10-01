//
// Created by james on 9/20/23.
//

#pragma once

#include "game/board/board_slot.hpp"
#include "game/board/board_stack.hpp"

#include "game/card_tank.hpp"
#include "game/layout/row_layout.hpp"

namespace game::board
{
    class BoardRow
    {
    public:
        struct TargetedCard { size_t idx; CardRef ref; };

        inline bool   isEmpty ()            const { return m_card_refs.empty(); }
        inline size_t numCards()            const { return m_card_refs.size(); }
        inline bool   hasCard (CardRef ref) const { return getIdx(ref) >= 0; }

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
        bool tryGetHoveredIndex(ut::vec2 const& mp, size_t& idx);

        bool tryGetTargetedCard(ut::vec2 const& mp, TargetedCard& target)
        {
            if (size_t idx; tryGetHoveredIndex(mp, idx))
            {
                target = { idx, m_card_refs[idx] };
                return true;
            }
            return false;
        }



        void setHighlight() { m_highlighted = true; }
        void clearHighlight() { m_highlighted = false; }

        CardRef giveCard(size_t idx);

        void addCard      (size_t idx, CardRef ref);
        void removeCard   (size_t idx);
        void removeCard   (CardRef ref);
        CardRef getCard(size_t idx) const;


        void layout (ut::rect const& b);
        void update (float dt);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug     ();

        int getTotalStrength();

        bool isTargeted(const ut::vec2 &vec);

    private:
        ut::rect            m_bounds;
        layout::RowLayout   m_layout_row;
        layout::RowLayout   m_layout_row_next;

        cardrefs_t          m_card_refs;

        bool m_highlighted = false;

        ssize_t getIdx(CardRef ref) const;
        void    rebuildLayout();
        void    arrangeRow();
    };
}