//
// Created by james on 12/5/23.
//

#pragma once

#include "game/board/board_row.hpp"
#include "game/board/board_card_list.hpp"

namespace game::board
{
    class BoardUnits : public BoardRow
    {
    public:
        void setHighlight();
        void clearHighlight();

        bool hasCardHighlight() const;
        void setCardHighlight(size_t idx);
        void clearCardHighlight();
        int getTotalStrength() const;
        int getMaxStrength() const;
        bool isTargeted(ut::vec2 const& mp);
        void drawAboveCards() override;

    protected:
        void onContainerChanged() override
        {
            clearCardHighlight();
            BoardRow::onContainerChanged();
        }

    private:
        bool    m_row_highlight = false;
        ssize_t m_card_highlight = -1;
    };

    class CardRefListUnits : public CardRefList
    {
    public:
        void setHighlight();
        void clearHighlight();

        bool hasCardHighlight() const;
        void setCardHighlight(size_t idx);
        void clearCardHighlight();
        int getTotalStrength() const;
        int getMaxStrength() const;
        bool isTargeted(ut::vec2 const& mp);
        void drawAboveCards() ;

    protected:
        void onContainerChanged() override;

    private:
        ut::rect m_bounds;
        RowLayout m_layout;

        bool    m_row_highlight = false;
        ssize_t m_card_highlight = -1;

        void rebuildLayout();
    };
}