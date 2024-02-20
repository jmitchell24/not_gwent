//
// Created by james on 9/20/23.
//

#pragma once

#include "game/board/board_slot.hpp"
#include "game/board/board_stack.hpp"

#include "game/card_tank.hpp"
#include "game/layout/row_layout.hpp"
#include "context.hpp"

namespace game::board
{
    using card_indices_t = std::vector<size_t>;
    using card_indices_param = std::vector<size_t> const&;


    class BoardRow
    {
    public:
        struct TargetedCard { size_t idx; CardRef ref; };

        virtual ~BoardRow(){}

        inline auto data () const { return m_card_refs.data(); }
        inline auto begin() const { return m_card_refs.begin(); }
        inline auto end  () const { return m_card_refs.end(); }

        bool isEmpty() const;
        size_t numCards() const;
        bool hasCard(CardRef ref) const;
        bool hasCardAny(cardrefs_param refs) const;
        bool hasCardAll(cardrefs_param refs) const;


        size_t tryGetNearestIndex(ut::vec2 const& mp) const;
        bool tryGetHoveredCard(ut::vec2 const &mp, CardRef &ref) const;
        bool tryGetHoveredIndex(ut::vec2 const &mp, size_t &idx) const;
        bool tryGetTargetedCard(ut::vec2 const &mp, TargetedCard &target) const;


        //
        // single-ref container functions
        //

        void addCard(size_t idx, CardRef ref);
        void removeCard(size_t idx);
        void removeCard(CardRef ref);
        CardRef giveCard(size_t idx);
        CardRef getCard(size_t idx) const;

        //
        // multi-ref container functions
        //

        void addCardMulti(size_t idx, cardrefs_param refs);
        void removeCardMulti(card_indices_param indices);
        cardrefs_t giveCardMulti(card_indices_param indices);
        cardrefs_t getCardMulti(card_indices_param indices) const;
        card_indices_t getCardIndices(cardrefs_param refs) const;
        void clear();

        //
        // template container functions
        //

        template<typename Predicate>
        inline card_indices_t getCardIndicesIf(Predicate &&p)
        {
            card_indices_t indices;
            for (size_t i = 0; i < m_card_refs.size(); ++i)
                if (p(m_card_refs[i]))
                    indices.push_back(i);
            return indices;
        }

        void layout(ut::rect const& b);
        void update(update_param) { }
        virtual void drawAboveCards() { }
        virtual void drawUnderCards() { }
        virtual void drawDebug() { }

    protected:
        ut::rect            m_bounds;
        RowLayout           m_layout_row;
        cardrefs_t          m_card_refs;

        virtual void onContainerChanged()
        {
            rebuildLayout();
        }

    private:
        //
        // helper functions
        //

        ssize_t getIdx(CardRef ref) const;
        void    rebuildLayout();
    };
}