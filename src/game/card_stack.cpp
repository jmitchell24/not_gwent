//
// Created by james on 9/6/23.
//

#include "game/card_stack.hpp"
#include "game/assets.hpp"
using namespace game;

//
// gfx
//
#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

CardStack::CardStack()
    : m_cards{}
{}

CardStack::CardStack(cardlist_t cards)
    : m_cards{std::move(cards)}
{}

void CardStack::setCards(cardlist_t cards)
{
    m_cards = std::move(cards);
}

void CardStack::push(Card const& card)
{
    m_cards.push_back(card);
}

Card CardStack::pop()
{
    assert(!m_cards.empty());

    Card card = m_cards.back();
    m_cards.pop_back();
    return card;
}

void CardStack::layout(ut::rect const& bounds)
{
    m_bounds = bounds;

    auto h = m_bounds.height();
    auto w = h / 1.43f;

    m_bounds_cards = m_bounds.anchorCCtoCC(m_bounds.fit(w, h));
    m_is_layout_ready = true;
}

void CardStack::update()
{
    assert(m_is_layout_ready);
}

void CardStack::draw()
{
    auto tex = textures::card_back();
    auto font = fonts::smallburgRegular128();

    drawShadow(m_bounds_cards, {5,5}, 5);
    drawTextureFit(tex, m_bounds_cards, colors::white);

    auto r = m_bounds_cards.anchorBCtoBC(m_bounds_cards.width()/2, m_bounds_cards.height()/2);
    drawRect(r, colors::black.withNormalA(0.4));
    drawTextBCtoBC(font, m_bounds_cards, m_bounds_cards.height()/3, PRINTER("%d", m_cards.size()), colors::ivory);

    VIRT_DEBUG(CardStack::m_bounds);
    VIRT_DEBUG(CardStack::m_bounds_cards);

    assert(m_is_layout_ready);
}