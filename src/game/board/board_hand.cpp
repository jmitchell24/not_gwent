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

//
// BoardHand -> Implementation
//

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

//
// CardRefListHand -> Implementation
//

bool CardRefListHand::hasCardHighlight() const
{
    return m_card_highlight >= 0;
}

void CardRefListHand::setCardHighlight(size_t idx)
{
    m_card_highlight = (ssize_t)idx;
}

void CardRefListHand::clearCardHighlight()
{
    m_card_highlight = -1;
}

void CardRefListHand::layout(rect const& bounds)
{
    m_bounds = bounds;
    rebuildLayout();

    for (size_t i = 0; i < m_refs.size(); ++i)
    {
        m_refs[i]->animBounds(m_layout.getRect(i));
    }
}

void CardRefListHand::drawAboveCards()
{
    if (auto idx = (size_t)m_card_highlight; hasCardHighlight())
    {
        drawRectOutline(m_refs[idx]->layout.getRect(), 2.0f, colors::red);
    }
}

void CardRefListHand::onContainerChanged()
{
    rebuildLayout();

    for (size_t i = 0; i < m_refs.size(); ++i)
    {
        m_refs[i]->animBounds(m_layout.getRect(i));
    }
}

void CardRefListHand::rebuildLayout()
{
    m_layout = RowLayout::create(
            m_bounds,
            CardLayout::widthFromHeight(m_bounds.height()),
            m_refs.size());
}