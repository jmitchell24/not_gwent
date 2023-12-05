#include "game/board/board_row.hpp"
using namespace game::board;
using namespace game;

#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
#include <ut/check.hpp>
using namespace ut;

//
// Helper Functions
//

inline static bool isNilAny(cardrefs_param refs)
{
    for (auto&& it: refs)
        if (it.isNil())
            return true;
    return false;
}

inline static bool hasIdxAll(card_indices_param indices, size_t sz)
{
    for (auto&& it: indices)
        if (!(it < sz))
            return false;
    return true;
}

//
// CardRow2 -> Implementation
//

//
// CardRow2 -> single-ref container functions
//

void BoardRow::addCard(size_t idx, CardRef ref)
{
    check(idx <= m_card_refs.size(), "invalid card index");
    check(!hasCard(ref), "card already in row");
    check(!ref.isNil(), "nil card");

    m_card_refs.insert(m_card_refs.begin()+ssize_t(idx), ref);
    rebuildLayout();
    arrangeRow();
}

void BoardRow::removeCard(size_t idx)
{
    check(idx <= m_card_refs.size(), "invalid card index");

    m_card_refs.erase(m_card_refs.begin() + ssize_t(idx));
    rebuildLayout();
    arrangeRow();
}

void BoardRow::removeCard(CardRef ref)
{
    ssize_t idx = getIdx(ref);
    check(idx >= 0, "card doesn't exist");
    removeCard(size_t(idx));
}

CardRef BoardRow::giveCard(size_t idx)
{
    check(idx <= m_card_refs.size(), "invalid card index");
    CardRef card = m_card_refs[idx];
    removeCard(idx);
    return card;
}

CardRef BoardRow::getCard(size_t idx) const
{
    check(idx <= m_card_refs.size(), "invalid card index");
    return m_card_refs[idx];
}

//
// CardRow2 -> multi-ref container functions
//

void BoardRow::addCardMulti(size_t idx, cardrefs_param refs)
{
    check(idx <= m_card_refs.size(), "invalid card index");
    check(hasCardAny(refs), "card already in row");
    check(isNilAny(refs), "nil card");

    m_card_refs.insert(m_card_refs.begin()+ssize_t(idx), refs.begin(), refs.end());
    rebuildLayout();
    arrangeRow();
}

void BoardRow::removeCardMulti(card_indices_param indices)
{
    check(hasIdxAll(indices, m_card_refs.size()), "invalid card index");

    if (indices.empty())
        return;

    for (auto&& it : indices)
        m_card_refs[it].reset();

    cardrefs_t new_refs;
    for (auto&& it : m_card_refs)
    {
        if (!it.isNil())
            new_refs.push_back(it);
    }
    m_card_refs.swap(new_refs);
    rebuildLayout();
    arrangeRow();
}

cardrefs_t BoardRow::giveCardMulti(card_indices_param indices)
{
    check(hasIdxAll(indices, m_card_refs.size()), "invalid card index");

    if (indices.empty())
        return {};

    cardrefs_t new_refs;
    cardrefs_t removed_refs;

    for (auto&& it : indices)
    {
        auto&& ref = m_card_refs[it];
        removed_refs.push_back(ref);
        ref.reset();
    }

    for (auto&& it : m_card_refs)
    {
        if (!it.isNil())
            new_refs.push_back(it);
    }

    m_card_refs.swap(new_refs);
    rebuildLayout();
    arrangeRow();

    return removed_refs;
}

cardrefs_t BoardRow::getCardMulti(card_indices_param indices) const
{
    check(hasIdxAll(indices, m_card_refs.size()), "invalid card index");

    cardrefs_t refs;
    for (auto&& it: indices)
        refs.push_back(m_card_refs[it]);
    return refs;
}

card_indices_t BoardRow::getCardIndices(cardrefs_param refs) const
{
    card_indices_t indices;
    for (auto&& it: m_card_refs)
        if (auto idx = getIdx(it))
            if (idx >= 0)
                indices.push_back(idx);
    return indices;
}

//
// game object functions
//

void BoardRow::setRowHighlight()
{
    m_row_highlight = true;
}

void BoardRow::clearRowHighlight()
{
    m_row_highlight = false;
}

void BoardRow::setCardHighlight(size_t idx)
{
    m_card_highlight = (ssize_t)idx;
}

void BoardRow::clearCardHighlight()
{
    m_card_highlight = -1;
}

int BoardRow::getTotalStrength() const
{
    int sum=0;
    for (auto&& it : m_card_refs)
        sum += (int)it->ng.asUnitCard().strength;
    return sum;
}

int BoardRow::getMaxStrength() const
{
    int max=0;
    for (auto&& it : m_card_refs)
        if (int strength = (int)it->ng.asUnitCard().strength; strength > max)
            max = strength;
    return max;
}

bool BoardRow::isTargeted(vec2 const& mp)
{
    return m_bounds.contains(mp);
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
    if (m_row_highlight)
    {
        drawRectOutline(m_bounds, 2.0f, colors::red);
    }

    if (auto idx = (size_t)m_card_highlight; idx < m_card_refs.size())
    {
        drawRectOutline(m_card_refs[idx]->layout.getRect(), 2.0f, colors::red);
    }
}

void BoardRow::drawUnderCards()
{

}

void BoardRow::drawDebug()
{

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
            m_bounds,
            layout::CardLayout::widthFromHeight(m_bounds.height()),
            m_card_refs.size());

//    m_layout_row_next = layout::RowLayout::create(
//            layout::RowLayout::DIR_CENTER, m_bounds,
//            layout::CardLayout::widthFromHeight(m_bounds.height()),
//            m_card_refs.size()+1);

    arrangeRow();
}

void BoardRow::arrangeRow()
{
    for (size_t i = 0; i < m_card_refs.size(); ++i)
    {
        m_card_refs[i]->move2(m_layout_row.getPos(i));
    }
}

