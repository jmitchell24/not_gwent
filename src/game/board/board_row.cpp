#include "game/board/board_row.hpp"
using namespace game::board;
using namespace game;

#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

//
// CardRow2 -> Implementation
//


bool BoardRow::tryGetHoveredCard(vec2 const& mp, CardRef& ref)
{
    if (size_t idx; m_layout_row.tryGetIndex(mp,idx))
    {
        ref = m_card_refs[idx];
        return true;
    }
    return false;
}

bool BoardRow::tryGetHoveredIndex(vec2 const& mp, size_t& idx)
{
    return m_layout_row.tryGetIndex(mp, idx);
}

CardRef BoardRow::giveCard(size_t idx)
{
    assert(idx < m_card_refs.size());
    CardRef card = m_card_refs[idx];
    removeCard(idx);
    return card;
}

void BoardRow::layout(ut::rect const& b)
{
    m_bounds = b;
    rebuildLayout();
}

void BoardRow::update(float dt)
{

}

void BoardRow::drawAboveCards()
{
    if (m_highlighted)
    {
        drawRectOutline(m_bounds, 2.0f, ut::colors::red);
    }
}

void BoardRow::drawUnderCards()
{

}

void BoardRow::drawDebug()
{

}

void BoardRow::addCard(size_t idx, CardRef ref)
{
    assert(idx <= m_card_refs.size());
    assert(!hasCard(ref));

    m_card_refs.insert(m_card_refs.begin()+ssize_t(idx), ref);
    rebuildLayout();
    arrangeRow();
}

void BoardRow::removeCard(size_t idx)
{
    assert(idx < m_card_refs.size());

    m_card_refs.erase(m_card_refs.begin() + ssize_t(idx));
    rebuildLayout();
    arrangeRow();
}

void BoardRow::removeCard(CardRef ref)
{
    ssize_t idx = getIdx(ref);
    assert(idx >= 0);
    removeCard(size_t(idx));
}

CardRef BoardRow::getCard(size_t idx) const
{
    assert(idx < m_card_refs.size());
    return m_card_refs[idx];
}

ssize_t BoardRow::getIdx(CardRef ref) const
{
    for (ssize_t i = 0; i < m_card_refs.size(); ++i)
        if (m_card_refs[i] == ref)
            return i;
    return -1;
}

void BoardRow::rebuildLayout()
{
    m_layout_row = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER,
            m_bounds,
            layout::CardLayout::widthFromHeight(m_bounds.height()),
            m_card_refs.size());

    m_layout_row_next = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER, m_bounds,
            layout::CardLayout::widthFromHeight(m_bounds.height()),
            m_card_refs.size()+1);
}

void BoardRow::arrangeRow()
{
    for (size_t i = 0; i < m_card_refs.size(); ++i)
    {
        m_card_refs[i]->move2(m_layout_row.getPos(i));
    }
}

int BoardRow::getTotalStrength()
{
    int sum=0;
    for (auto&& it : m_card_refs)
        sum += (int)it->ng.asUnitCard().strength;
    return sum;
}

bool BoardRow::isTargeted(vec2 const& mp)
{
    return m_bounds.contains(mp);
}
