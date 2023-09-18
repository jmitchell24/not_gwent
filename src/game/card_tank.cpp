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

Card& CardTank::getCard(cardid_t id)
{
    return m_cards[indexFromId(id)];
}

Card& CardTank::addCard(Card card)
{
    card.m_id     = nextIdValue();
    card.m_order  = nextOrderValue();

    m_map[card.m_id.value] = m_cards.size();
    m_cards.push_back(card);

    return m_cards.back();
}

void CardTank::removeCard(cardid_t id)
{
    m_cards.erase(m_cards.begin() + (ssize_t)indexFromId(id));
    rebuildMap();

    m_old_ids.push_back(id);
}

void CardTank::elevateCard(cardid_t id)
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

    for (auto&& it: m_cards)
    {
        auto r = it.layout.getRect();
        drawTextTCtoTC(r, 15, PRINTER("%d", it.m_id.value), colors::salmon);
        drawTextBCtoBC(r, 15, PRINTER("%d", it.m_order), colors::cyan);
    }
}

void CardTank::drawDebug()
{
    ImGui::LabelText("m_next_order_value", "%zu", m_next_order_value);
    ImGui::LabelText("m_next_id_value", "%zu", m_next_id_value.value);

    ImGui::LabelText("m_cards.size()", "%zu", m_cards.size());
    ImGui::LabelText("m_map.size()", "%zu", m_map.size());
    ImGui::LabelText("m_old_ids.size()", "%zu", m_old_ids.size());


}

size_t CardTank::indexFromId(cardid_t id)
{
    assert(id.isValid());

    auto it = m_map.find(id.value);

    assert(it != m_map.end());
    assert(it->second < m_cards.size());

    return it->second;
}

size_t CardTank::nextOrderValue()
{
    return m_next_order_value++;
}

cardid_t CardTank::nextIdValue()
{
    if (m_old_ids.empty())
        return m_next_id_value++;
    cardid_t id = m_old_ids.back();
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

cardidlist_t CardTank::addTestCards(size_t n)
{
    cardidlist_t cardids;
    for (size_t i = 0; i < n; ++i)
        cardids.push_back(addTestCard().m_id);
    return cardids;
}