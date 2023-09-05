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
    m_slots         {}
{
    for (auto&& it : cards)
        m_slots.push_back({it, nextOrderValue()});
    refreshDrawIndices();
}

void CardList::layout(rect const& bounds)
{
    m_bounds        = bounds;

    m_card_height   = m_bounds.height();
    //m_card_width    = m_card_height / 1.88f; // aspect ratio of card images from Witcher 3
    m_card_width    = m_card_height / 1.43f;

    m_layout_ghosted = CardLayout::create(m_direction, m_bounds, m_card_width, m_slots.size() + 1);
    m_layout_hovered = CardLayout::create(m_direction, m_bounds, m_card_width, m_slots.size());

    m_idx_hovered   = -1;
    m_idx_ghosted   = -1;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif

    for (size_t i = 0; i < m_slots.size(); ++i)
    {
        auto&& it = m_slots[i].card;
        it.layout({m_card_width, m_card_height});
        it.setPosition(m_layout_hovered.getPos(i));
    }
}

void CardList::update()
{
    assert(m_is_layout_ready);

    for (auto&& it: m_slots)
        it.card.update();
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
    {
        auto&& slot = m_slots[it];
        slot.card.draw();
    }

    for (auto&& it: m_draw_indices)
    {
        auto&& slot = m_slots[it];
        VIRT.drawTextTLtoTL(slot.card.drawBounds().shrunk(10).withHeight(30), PRINTER("%d", slot.order), colors::white);
    }

//    for (size_t i = 0; i < m_cards.size(); ++i)
//    {
//        if (m_idx_hovered == i)
//            continue;
//        m_cards[i].draw();
//    }
//
//    if (m_idx_hovered >= 0)
//        m_cards[m_idx_hovered].draw();

//    if (m_idx_ghosted >= 0)
//    {
//        auto r = m_layout_ghosted.getRect(m_idx_ghosted);
//        VIRT.drawRectangle(r, colors::greenyellow.withNormalA(0.5f));
//    }
}

void CardList::setGhost(size_t idx)
{
    assert(idx <= m_slots.size());
    if (m_idx_ghosted != idx)
    {
        m_idx_ghosted = (ssize_t)idx;

        for (size_t i = 0; i < m_idx_ghosted; ++i)
        {
            m_slots[i].card.animNudge(m_layout_ghosted.getPos(i));
        }

        for (size_t i = m_idx_ghosted; i < m_slots.size(); ++i)
        {
            m_slots[i].card.animNudge(m_layout_ghosted.getPos(i + 1));
        }
    }
}

void CardList::clearGhost()
{
    if (m_idx_ghosted >= 0)
    {
        m_idx_ghosted = -1;

        for (size_t i = 0; i < m_slots.size(); ++i)
        {
            m_slots[i].card.animNudge(m_layout_hovered.getPos(i));
        }
    }
}

void CardList::setHover(size_t idx)
{
    assert(idx < m_slots.size());
    hover((ssize_t)idx);
}

void CardList::clearHover()
{
    hover(-1);
}

void CardList::addCard(size_t idx, Card const& card)
{
    assert(m_is_layout_ready);
    assert(idx <= m_slots.size());

    clearHover();
    clearGhost();

    m_slots.insert(m_slots.begin() + ssize_t(idx), {card, nextOrderValue()});
    m_idx_hovered = ssize_t(idx);

    refreshCardPositions(ssize_t(idx));
    refreshDrawIndices();
}

Card CardList::removeCard(size_t idx)
{
    assert(m_is_layout_ready);
    assert(idx < m_slots.size());

    clearHover();
    clearGhost();

    Card card = m_slots[idx].card;
    m_slots.erase(m_slots.begin()+ssize_t(idx));

    refreshCardPositions();
    refreshDrawIndices();

    return card;
}

size_t CardList::nextOrderValue()
{
    return m_next_order_value++;
}



void CardList::refreshDrawIndices()
{
    // Recreate list of indices using the slot order value for sorting.

    m_draw_indices.clear();
    for (size_t i = 0; i < m_slots.size(); ++i)
        m_draw_indices.push_back(i);

    std::sort(m_draw_indices.begin(), m_draw_indices.end(), [&](auto&& a, auto&& b)
    {
        return m_slots[a].order < m_slots[b].order;
    });

    // Normalize the slot order values, and reset m_next_order_value.
    // Reduces risk of overflow, but is probably unnecessary when size_t is 32 or more bits.

    for (size_t i = 0; i < m_draw_indices.size(); ++i)
        m_slots[m_draw_indices[i]].order = i;
    m_next_order_value = m_draw_indices.size();
}


//TODO: nudge everything except card on top (As per order). place_idx not necessary
void CardList::refreshCardPositions(ssize_t place_idx)
{
    assert(m_is_layout_ready);

    m_layout_ghosted = CardLayout::create(m_direction, m_bounds, m_card_width, m_slots.size() + 1);
    m_layout_hovered = CardLayout::create(m_direction, m_bounds, m_card_width, m_slots.size());

    for (size_t i = 0; i < m_slots.size(); ++i)
    {
        if (i != place_idx)
            m_slots[i].card.animNudge(m_layout_hovered.getPos(i));
    }
}

void CardList::hover(ssize_t idx)
{
    assert(idx < (ssize_t)m_slots.size());
    if (m_idx_hovered != idx)
    {
        if (m_idx_hovered >= 0)
            m_slots[m_idx_hovered].card.animDrop();

        m_idx_hovered = (ssize_t)idx;

        if (m_idx_hovered >= 0)
        {
            m_slots[m_idx_hovered].card.animPeek();

            m_slots[m_idx_hovered].order = nextOrderValue();
            refreshDrawIndices();
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