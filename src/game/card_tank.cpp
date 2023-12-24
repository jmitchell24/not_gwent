//
// Created by james on 8/16/23.
//

#include <ut/random.hpp>
#include "assert.hpp"

#include "game/card_tank.hpp"
#include "gfx/gfx_draw.hpp"

using namespace game;

#include "rlImGui/imgui/imgui.h"

//
// gfx
//
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
= default;

CardTank& CardTank::instance()
{
    static CardTank x;
    return x;
}

bool CardTank::hasCard(CardID id) const
{
    return m_id_map.find(id.value) != m_id_map.end();
}

Card& CardTank::getCard(CardID id)
{
    return getCard(getIndex(id));
}

Card& CardTank::getCard(Index idx)
{
    return m_layers[idx.layer_idx].cards[idx.card_idx];
}

Card& CardTank::addCard(Card card, CardLayer layer)
{
    check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");

    auto& l = m_layers[layer];

    card.m_id     = CardID{nextIdValue()};
    card.m_order  = l.nextOrderValue();

    m_id_map[card.m_id.value] = { (size_t)layer, l.cards.size() };
    l.cards.push_back(card);
    return l.cards.back();
}

cardrefs_t CardTank::addCardMulti(cardlist_param cards, CardLayer layer)
{
    check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");

    cardrefs_t refs;

    auto& l = m_layers[layer];
    for (auto it : cards)
    {
        it.m_id     = CardID{nextIdValue()};
        it.m_order  = l.nextOrderValue();

        m_id_map[it.m_id.value] = { (size_t)layer, l.cards.size() };
        l.cards.push_back(it);
        refs.push_back(l.cards.back().ref());
    }

    return refs;
}

void CardTank::removeCard(CardID id)
{
    auto idx = getIndex(id);

    auto& l = m_layers[idx.layer_idx];
    l.cards.erase(l.cards.begin() + (ptrdiff_t) idx.card_idx);
    l.reorderCards();

    m_id_map.erase(id.value);
    assignIdValues(idx.layer_idx);

    m_old_ids.push_back(id.value);
}

void CardTank::removeCardMulti(cardrefs_param refs)
{
    nopath_impl;
}

void CardTank::elevateCard(CardID id)
{
    auto idx = getIndex(id);
    auto& l = m_layers[idx.layer_idx];
    auto& c = l.cards[idx.card_idx];

    c.m_order = l.nextOrderValue();
    l.reorderCards();
    assignIdValues(idx.layer_idx);
}

void CardTank::update(update_param u)
{
    for (auto&& l: m_layers)
        for (auto&& c : l.cards)
            c.update(u.frame_time);
}

void CardTank::draw(CardLayer layer)
{
    check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");

    for (auto&& it : m_layers[layer].cards)
        it.draw();
}

void CardTank::drawDebug()
{
    using namespace ImGui;

    for (size_t i = 0; i < m_layers.size(); ++i)
    {
        auto& l = m_layers[i];
        Text("Layer %zu: cnt=%zu, next_order_value=%zu",
             i, l.cards.size(), l.next_order_value);
    }

    LabelText("Next ID Value", "%zu", m_next_id_value);
    LabelText("ID Map Size", "%zu", m_id_map.size());
    LabelText("Old IDs Size", "%zu", m_old_ids.size());
}

CardTank::Index CardTank::getIndex(CardID id)
{
    auto it = m_id_map.find(id.value);
    check(it != m_id_map.end(), "bad card id");

    auto&& idx = it->second;
    check(idx.layer_idx < m_layers.size(), "bad layer value");
    check(idx.card_idx < m_layers[idx.layer_idx].cards.size(), "bad index value");

    return idx;
}

size_t CardTank::Layer::nextOrderValue()
{
    return next_order_value++;
}

CardTank::id_type CardTank::nextIdValue()
{
    if (m_old_ids.empty())
        return m_next_id_value++;
    auto id = m_old_ids.back();
    m_old_ids.pop_back();
    return id;
}

void CardTank::assignIdValues(size_t layer_idx)
{
    check(layer_idx < m_layers.size(), "bad layer index");

    auto& layer = m_layers[layer_idx];
    for (size_t i = 0; i < layer.cards.size(); ++i)
        m_id_map[layer.cards[i].m_id.value] = { layer_idx, i };
}

void CardTank::Layer::normalizeOrderValues()
{
    // Normalize slot order values, and reset m_next_order_value.
    // Reduces risk of overflow, but is probably unnecessary when size_t is 32 or more bits.
    for (size_t i = 0; i < cards.size(); ++i)
        cards[i].m_order = i;
    next_order_value = cards.size();
}

void CardTank::Layer::reorderCards()
{
    if (cards.empty())
        return;

    std::sort(cards.begin(), cards.end(),
        [](auto&& a, auto&& b) { return a.m_order < b.m_order; });

    normalizeOrderValues();
}

#if 0
void CardTank::reorderCards(size_t layer)
{
    check(layer < m_layer_map.size(), "bad layer value");

    if (m_cards.empty())
        return;

    auto beg = m_cards.begin() + (ptrdiff_t)m_layer_map[layer].first;
    auto end = m_cards.end() + (ptrdiff_t)m_layer_map[layer].second;

    std::sort(beg, end, [](auto&& a, auto&& b)
    {
        return a.m_order < b.m_order;
    });
}
#endif

Card& CardTank::addTestCard(CardLayer layer)
{
    auto ng_id = (ng::CardID)RNG.nexti((int)ng::CARD_COUNT_);

    Card card;
    card.ng = ng::getCard(ng_id);
    card.assets = Card::Assets::fromNgCard(card.ng);

    return addCard(card, layer);
}

cardrefs_t CardTank::addTestCards(size_t n, CardLayer layer)
{
    cardrefs_t list;
    for (size_t i = 0; i < n; ++i)
        list.emplace_back(addTestCard(layer).ref());
    return list;
}

