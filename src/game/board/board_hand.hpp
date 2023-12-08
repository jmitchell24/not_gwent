//
// Created by james on 12/5/23.
//

#include "game/board/board_row.hpp"

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


}