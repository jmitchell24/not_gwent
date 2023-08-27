//
// Created by james on 8/16/23.
//

#include "assert.hpp"

#include "game/cardlist.hpp"
using namespace game;

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

size_t constexpr static VIRT_WIDTH  = 720;
size_t constexpr static VIRT_HEIGHT = 1280;
size_t constexpr static VIRT_PAD    = 10;

//
// CardList -> Implementation
//

CardList::CardList(CardLayout::Direction direction, cardlist_t cards) :
    m_bounds        {},
    m_card_width    {},
    m_card_height   {},
    m_direction     {direction},
    m_cards         {std::move(cards)}
{
    rebuildDrawIndices();
}

void CardList::layout(rect const& bounds)
{
    m_bounds        = bounds;

    m_card_height   = m_bounds.height();
    //m_card_width    = m_card_height / 1.88f; // aspect ratio of card images from Witcher 3
    m_card_width    = m_card_height / 1.43f;

    m_layout_ghosted = CardLayout::create(m_direction, m_bounds, m_card_width, m_cards.size() + 1);
    m_layout_hovered = CardLayout::create(m_direction, m_bounds, m_card_width, m_cards.size());

    m_idx_hovered   = -1;
    m_idx_ghosted   = -1;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif

    for (size_t i = 0; i < m_cards.size(); ++i)
    {
        auto&& it = m_cards[i];
        it.layout({m_card_width, m_card_height});
        it.setPosition(m_layout_hovered.getPos(i));
    }
}

void CardList::update()
{
    assert(m_is_layout_ready);

    for (auto&& it: m_cards)
        it.update();
}

void CardList::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(CardList::m_bounds);
    VIRT_DEBUG(CardList::m_layout_ghosted.bounds);
    VIRT_DEBUG(CardList::m_layout_hovered.bounds);

    VIRT.drawRectangleLines(m_bounds, 2.0f, colors::white);
    VIRT.drawRectangle(m_bounds, colors::darkslategrey);

    for (auto&& it: m_draw_indices)
        m_cards[it].draw();

//    for (size_t i = 0; i < m_cards.size(); ++i)
//    {
//        if (m_idx_hovered == i)
//            continue;
//        m_cards[i].draw();
//    }
//
//    if (m_idx_hovered >= 0)
//        m_cards[m_idx_hovered].draw();

    if (m_idx_ghosted >= 0)
    {
        auto r = m_layout_ghosted.getRect(m_idx_ghosted);
        VIRT.drawRectangle(r, colors::greenyellow.withNormalA(0.5f));
    }
}

void CardList::setGhost(size_t idx)
{
    assert(idx <= m_cards.size());
    if (m_idx_ghosted != idx)
    {
        m_idx_ghosted = (ssize_t)idx;

        for (size_t i = 0; i < m_idx_ghosted; ++i)
        {
            m_cards[i].animNudge(m_layout_ghosted.getPos(i));
        }

        for (size_t i = m_idx_ghosted; i < m_cards.size(); ++i)
        {
            m_cards[i].animNudge(m_layout_ghosted.getPos(i + 1));
        }
    }
}

void CardList::clearGhost()
{
    if (m_idx_ghosted >= 0)
    {
        m_idx_ghosted = -1;

        for (size_t i = 0; i < m_cards.size(); ++i)
        {
            m_cards[i].animNudge(m_layout_hovered.getPos(i));
        }
    }
}

void CardList::setHover(size_t idx)
{
    assert(idx < m_cards.size());
    hover((ssize_t)idx);
}

void CardList::clearHover()
{
    hover(-1);
}

//bool CardList::tryGetHoverIndex(vec2 const& mp, size_t& idx) const
//{
//    assert(m_is_layout_ready);
//    return m_layout_hovered.tryGetIndex(mp, idx);
//}
//
//bool CardList::tryGetGhostIndex(vec2 const& mp, size_t& idx) const
//{
//    assert(m_is_layout_ready);
//    return m_layout_ghosted.tryGetIndex(mp, idx);
//}

void CardList::addCard(size_t idx, Card const& card)
{
    assert(m_is_layout_ready);
    assert(idx <= m_cards.size());

    clearHover();
    clearGhost();

    Card& it = *m_cards.insert(m_cards.begin() + idx, card);
    //it.layout({m_card_width, m_card_height});
    it.animDrop();

    m_idx_hovered = idx;

    updateCardPositions(idx);
    rebuildDrawIndices();
    replaceDrawIdx(idx);
}

Card CardList::removeCard(size_t idx)
{
    assert(m_is_layout_ready);
    assert(idx < m_cards.size());

    clearHover();
    clearGhost();

    Card card = m_cards[idx];
    m_cards.erase(m_cards.begin()+idx);
    updateCardPositions();
    rebuildDrawIndices();

    return card;
}

bool CardList::hasDrawIdx(size_t idx)
{
    for (auto&& it: m_draw_indices)
        if (it == idx)
            return true;
    return false;
}

void CardList::addDrawIdx(size_t idx)
{
    assert(!hasDrawIdx(idx));
    m_draw_indices.push_back(idx);

}

void CardList::rebuildDrawIndices()
{
    m_draw_indices.clear();
    for (size_t i = 0; i < m_cards.size(); ++i)
        m_draw_indices.push_back(i);
}

void CardList::replaceDrawIdx(size_t idx)
{
    removeDrawIdx(idx);
    addDrawIdx(idx);
}

void CardList::removeDrawIdx(size_t idx)
{
    assert(hasDrawIdx(idx));
    auto it = find(m_draw_indices.begin(), m_draw_indices.end(), idx);
    m_draw_indices.erase(it);
}

void CardList::updateCardPositions(ssize_t place_idx)
{
    assert(m_is_layout_ready);

    m_layout_ghosted = CardLayout::create(m_direction, m_bounds, m_card_width, m_cards.size() + 1);
    m_layout_hovered = CardLayout::create(m_direction, m_bounds, m_card_width, m_cards.size());

    for (size_t i = 0; i < m_cards.size(); ++i)
    {
        if (i == place_idx)
            m_cards[i].animPlace(m_layout_hovered.getPos(i));
        else
            m_cards[i].animNudge(m_layout_hovered.getPos(i));
    }
}

void CardList::hover(ssize_t idx)
{
    assert(idx < (ssize_t)m_cards.size());
    if (m_idx_hovered != idx)
    {
        if (m_idx_hovered >= 0)
            m_cards[m_idx_hovered].animDrop();

        m_idx_hovered = (ssize_t)idx;

        if (m_idx_hovered >= 0)
        {
            m_cards[m_idx_hovered].animPeek();
            replaceDrawIdx(idx);
        }
    }
}

//
// CardList::CardLayout -> Implementation
//

CardLayout CardLayout::create(Direction direction, rect const& bounds, float card_width, size_t card_count)
{
    if (float w = card_width * float(card_count); w < bounds.width())
    {
        switch (direction)
        {
            case DIR_LEFT  : return { bounds.anchorTLtoTL(w, bounds.height()), card_count, card_width, card_width };
            case DIR_CENTER: return { bounds.anchorCCtoCC(w, bounds.height()), card_count, card_width, card_width };

            default: assert_case(Direction);
        }

        return { {}, 0, 0.0f, 0.0f };
    }

    return { bounds, card_count, card_width, (bounds.width() - card_width) / float(card_count-1) };
}

bool CardLayout::tryGetIndex(ut::vec2 const& mp, size_t& idx) const
{
    if (bounds.contains(mp))
    {
        auto mp_min   = bounds.min.x;
        auto mp_max   = bounds.max.x;
        auto mp_width = mp_max - mp_min;

        idx = size_t( ((mp.x - mp_min) / (mp_width)) * float(card_count) );
        return true;
    }
    return false;
}

vec2 CardLayout::getPos(size_t idx) const
{
    return bounds.pos().withOffsetX(card_gap * float(idx));
}

rect CardLayout::getRect(size_t idx) const
{
    return bounds.withWidth(card_width).withOffsetX(card_gap * float(idx));
}