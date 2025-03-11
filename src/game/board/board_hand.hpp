//
// Created by james on 12/5/23.
//

#pragma once

#include "game/board/board_row.hpp"
#include "game/board/board_card_list.hpp"

namespace game::board
{
    class BoardHand : public BoardRow
    {
    public:
        bool hasCardHighlight() const;
        void setCardHighlight(size_t idx);
        void clearCardHighlight();
        void drawAboveCards() override;
    private:
        ssize_t m_card_highlight = -1;



    protected:
        void onContainerChanged() override
        {
            clearCardHighlight();
            BoardRow::onContainerChanged();
        }
    };

    class CardRefListHand : public CardRefList
    {
    public:
        bool hasCardHighlight() const;
        void setCardHighlight(size_t idx);
        void clearCardHighlight();

        void layout(ut::rect const& bounds);
        void drawAboveCards();

    protected:
        void onContainerChanged() override;

    private:
        ut::rect m_bounds;
        RowLayout m_layout;
        ssize_t m_card_highlight = -1;

        void rebuildLayout();
    };
}