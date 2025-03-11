//
// Created by james on 12/5/23.
//

#include "game/board/board_units.hpp"
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
// BoardUnits -> Implementation
//

void BoardUnits::setHighlight()
{
    m_row_highlight = true;
}

void BoardUnits::clearHighlight()
{
    m_row_highlight = false;
}

bool BoardUnits::hasCardHighlight() const
{
    return m_card_highlight >= 0;
}

void BoardUnits::setCardHighlight(size_t idx)
{
    m_card_highlight = (ssize_t)idx;
}

void BoardUnits::clearCardHighlight()
{
    m_card_highlight = -1;
}

int BoardUnits::getTotalStrength() const
{
    int sum=0;
    for (auto&& it : m_card_refs)
        sum += (int)it->ng.asUnitCard().strength;
    return sum;
}

int BoardUnits::getMaxStrength() const
{
    int max=0;
    for (auto&& it : m_card_refs)
        if (int strength = (int)it->ng.asUnitCard().strength; strength > max)
            max = strength;
    return max;
}

bool BoardUnits::isTargeted(ut::vec2 const& mp)
{
    return m_bounds.contains(mp);
}

void BoardUnits::drawAboveCards()
{
    if (m_row_highlight)
    {
        drawRectOutline(m_bounds, 2.0f, colors::red);
    }

    if (auto idx = (size_t)m_card_highlight; hasCardHighlight())
    {
        drawRectOutline(m_card_refs[idx]->layout.getRect(), 2.0f, colors::red);
    }
}

//
// CardRefListUnits -> Implementation
//

void CardRefListUnits::setHighlight()
{
    m_row_highlight = true;
}

void CardRefListUnits::clearHighlight()
{
    m_row_highlight = false;
}

bool CardRefListUnits::hasCardHighlight() const
{
    return m_card_highlight >= 0;
}

void CardRefListUnits::setCardHighlight(size_t idx)
{
    m_card_highlight = (ssize_t)idx;
}

void CardRefListUnits::clearCardHighlight()
{
    m_card_highlight = -1;
}

int CardRefListUnits::getTotalStrength() const
{
    int sum=0;
    for (auto&& it : m_refs)
        sum += (int)it->ng.asUnitCard().strength;
    return sum;
}

int CardRefListUnits::getMaxStrength() const
{
    int max=0;
    for (auto&& it : m_refs)
        if (int strength = (int)it->ng.asUnitCard().strength; strength > max)
            max = strength;
    return max;
}

bool CardRefListUnits::isTargeted(ut::vec2 const& mp)
{
    return m_bounds.contains(mp);
}

void CardRefListUnits::drawAboveCards()
{
    if (m_row_highlight)
    {
        drawRectOutline(m_bounds, 2.0f, colors::red);
    }

    if (auto idx = (size_t)m_card_highlight; hasCardHighlight())
    {
        drawRectOutline(m_refs[idx]->layout.getRect(), 2.0f, colors::red);
    }
}

void CardRefListUnits::onContainerChanged()
{
    rebuildLayout();

    for (size_t i = 0; i < m_refs.size(); ++i)
    {
        m_refs[i]->animBounds(m_layout.getRect(i));
    }
}

void CardRefListUnits::rebuildLayout()
{
    m_layout = RowLayout::create(
            m_bounds,
            CardLayout::widthFromHeight(m_bounds.height()),
            m_refs.size());
}