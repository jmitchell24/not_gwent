//
// Created by james on 8/16/23.
//

#include <ut/random.hpp>
#include "assert.hpp"

#include "game/card_tank.hpp"
#include "gfx/gfx_draw.hpp"

using namespace game;

#include "rlImGui/imgui/imgui_extra.hpp"

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "ng/ng_card_data.hpp"

using namespace gfx;

//
// ut
//
using namespace ut;

//
// CardTank -> Implementation
//

CardTank::CardTank()
{}

CardTank& CardTank::instance()
{
    static CardTank x;
    return x;
}

bool CardTank::hasCard(CardID id) const
{
    return m_map.find(id.value) != m_map.end();
}

Card& CardTank::getCard(CardID id)
{
    return m_cards[getIndex(id)];
}

Card& CardTank::addCard(Card card)
{
    card.m_id     = CardID{nextIdValue()};
    card.m_order  = nextOrderValue();

    m_map[card.m_id.value] = m_cards.size();
    m_cards.push_back(card);

    return m_cards.back();
}

void CardTank::removeCard(CardID id)
{
    m_cards.erase(m_cards.begin() + (ssize_t)getIndex(id));
    rebuildMap();

    m_old_ids.push_back(id.value);
}



void CardTank::elevateCard(CardID id)
{
    getCard(id).m_order = nextOrderValue();
    reorderCards();
}

void CardTank::update(float dt)
{
    for (auto&& it: m_cards)
        it.update(dt);
}

void CardTank::draw()
{
    for (auto&& it: m_cards)
    {
        it.draw();
    }

    if (VIRT.isRectDebugEnabled())
    {
        for (auto&& it: m_cards)
        {
            auto r = it.layout.getRect();
            drawTextTCtoTC(r, 15, PRINTER("%d", it.m_id.value), colors::salmon);
            drawTextBCtoBC(r, 15, PRINTER("%d", it.m_order), colors::cyan);

            if (it.isMoving())
            {
                drawRect(it.getDrawRect(), colors::red.withNormalA(0.33f));
            }
        }


    }


}

void CardTank::drawDebug()
{
    ImGui::LabelText("m_next_order_value", "%zu", m_next_order_value);
    ImGui::LabelText("m_next_id_value", "%zu", m_next_id_value);

    ImGui::LabelText("m_cards.size()", "%zu", m_cards.size());
    ImGui::LabelText("m_map.size()", "%zu", m_map.size());
    ImGui::LabelText("m_old_ids.size()", "%zu", m_old_ids.size());


}

size_t CardTank::getIndex(CardID id)
{
    auto it = m_map.find(id.value);

    assert(it != m_map.end());
    assert(it->second < m_cards.size());

    return it->second;
}

bool CardTank::tryGetIndex(CardID id, size_t &idx)
{
    if (auto it = m_map.find(id.value); it != m_map.end())
    {
        idx = it->second;
        return true;
    }
    return false;
}

size_t CardTank::nextOrderValue()
{
    return m_next_order_value++;
}

CardTank::id_type CardTank::nextIdValue()
{
    if (m_old_ids.empty())
        return m_next_id_value++;
    auto id = m_old_ids.back();
    m_old_ids.pop_back();
    return id;
}

void CardTank::rebuildMap()
{
    m_map.clear();
    for (size_t i = 0; i < m_cards.size(); ++i)
        m_map[m_cards[i].m_id.value] = i;
}

void CardTank::reorderCards()
{
    std::sort(m_cards.begin(), m_cards.end(), [&](auto&& a, auto&& b)
    {
        return a.m_order < b.m_order;
    });

    rebuildMap();

    // Normalize slot order values, and reset m_next_order_value.
    // Reduces risk of overflow, but is probably unnecessary when size_t is 32 or more bits.

    for (size_t i = 0; i < m_cards.size(); ++i)
        m_cards[i].m_order = i;
    m_next_order_value = m_cards.size();
}

Card& CardTank::addTestCard()
{
    auto ng_id = (ng::CardID)RNG.nexti((int)ng::CARD_COUNT_);

    Card card;
    card.ng = ng::getCard(ng_id);
    card.assets = Card::Assets::fromNgCard(card.ng);

    return addCard(card);
}

cardrefs_t CardTank::addTestCards(size_t n)
{
    cardrefs_t list;
    for (size_t i = 0; i < n; ++i)
        list.emplace_back(addTestCard().ref());
    return list;
}