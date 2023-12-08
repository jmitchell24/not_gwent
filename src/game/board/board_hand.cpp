//
// Created by james on 12/5/23.
//

#include "game/board/board_hand.hpp"
using namespace game::board;
using namespace game;

//
// gfx
//

#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
#include <ut/check.hpp>
using namespace ut;

bool BoardHand::hasCardHighlight() const
{
    return m_card_highlight >= 0;
}

void BoardHand::setCardHighlight(size_t idx)
{
    m_card_highlight = (ssize_t)idx;
}

void BoardHand::clearCardHighlight()
{
    m_card_highlight = -1;
}

void BoardHand::drawAboveCards()
{
    if (auto idx = (size_t)m_card_highlight; hasCardHighlight())
    {
        drawRectOutline(m_card_refs[idx]->layout.getRect(), 2.0f, colors::red);
    }
}
