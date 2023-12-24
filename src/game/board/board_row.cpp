#include "game/board/board_row.hpp"
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

bool BoardRow::isEmpty() const
{ return m_card_refs.empty(); }

size_t BoardRow::numCards() const
{ return m_card_refs.size(); }

bool BoardRow::hasCard(CardRef ref) const
{ return getIdx(ref) >= 0; }

bool BoardRow::hasCardAny(cardrefs_param refs) const
{
    for (auto&& it: refs)
        if (hasCard(it))
            return true;
    return false;
}

bool BoardRow::hasCardAll(cardrefs_param refs) const
{
    for (auto &&it: refs)
        if (!hasCard(it))
            return false;
    return true;
}



bool BoardRow::tryGetHoveredCard(vec2 const &mp, CardRef &ref) const
{
    if (size_t idx; m_layout_row.tryGetIndex(mp, idx))
    {
        ref = m_card_refs[idx];
        return true;
    }
    return false;
}

bool BoardRow::tryGetHoveredIndex(vec2 const &mp, size_t &idx) const
{
    return m_layout_row.tryGetIndex(mp, idx);
}

bool BoardRow::tryGetTargetedCard(vec2 const &mp, TargetedCard &target) const
{
    if (size_t idx; tryGetHoveredIndex(mp, idx))
    {
        target = {idx, m_card_refs[idx]};
        return true;
    }
    return false;
}

void BoardRow::addCard(size_t idx, CardRef ref)
{
    check(idx <= m_card_refs.size(), "invalid card index");
    check(!hasCard(ref), "card already in row");
    check(!ref.isNil(), "nil card");

    m_card_refs.insert(m_card_refs.begin()+ssize_t(idx), ref);
    onContainerChanged();
}

void BoardRow::removeCard(size_t idx)
{
    check(idx <= m_card_refs.size(), "invalid card index");

    m_card_refs.erase(m_card_refs.begin() + ssize_t(idx));
    onContainerChanged();
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
    check(!hasCardAny(refs), "card already in row");
    check(!isNilAny(refs), "nil card");

    m_card_refs.insert(m_card_refs.begin()+ssize_t(idx), refs.begin(), refs.end());
    onContainerChanged();
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
    onContainerChanged();
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
    onContainerChanged();

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

void BoardRow::clear()
{
    m_card_refs.clear();
}

void BoardRow::layout(ut::rect const& b)
{
    m_bounds = b;
    rebuildLayout();
}

ssize_t BoardRow::getIdx(CardRef ref) const
{
    for (ssize_t i = 0; i < m_card_refs.size(); ++i)
        if (m_card_refs[i] == ref)
            return i;
    return -1;
}

size_t BoardRow::tryGetNearestIndex(vec2 const& mp) const
{
    size_t index=0;
    float distance = FLT_MAX;

    for (size_t i = 0; i < m_layout_row.card_count; ++i)
    {
        if (float d = m_layout_row.getRect(i).center().distance(mp); d < distance)
        {
            distance = d;
            index = i;
        }
    }

    return index;
}

void BoardRow::rebuildLayout()
{
    m_layout_row = RowLayout::create(
            m_bounds,
            CardLayout::widthFromHeight(m_bounds.height()),
            m_card_refs.size());

//    m_layout_row_next = layout::RowLayout::create(
//            layout::RowLayout::DIR_CENTER, m_bounds,
//            layout::CardLayout::widthFromHeight(m_bounds.height()),
//            m_card_refs.size()+1);

    for (size_t i = 0; i < m_card_refs.size(); ++i)
    {
        m_card_refs[i]->animBounds(m_layout_row.getRect(i));
    }
}



