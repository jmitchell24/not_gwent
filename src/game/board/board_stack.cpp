#include "game/board/board_stack.hpp"
#include "game/assets.hpp"
using namespace game::board;
using namespace game;

//
// ng
//
#include "ng/ng_card_data.hpp"

//
// gfx
//
#include "gfx/gfx_draw.hpp"
using namespace gfx;

//
// ut
//
#include <ut/random.hpp>
using namespace ut;

//
// CardStack2 -> Implementation
//

Texture2D getTypeTexture(BoardStack::Type type)
{
    switch (type)
    {
        case BoardStack::YARD: return textures::card_back_neutral();
        case BoardStack::DECK: return textures::card_back_graveyard();
        default:assert_case(BoardStack::Type);
    }
    return {};
}

BoardStack::BoardStack(Type type) :
    m_bounds        { },
    m_card_ngs      { },
    m_card_back     { getTypeTexture(type) },
    m_spinner       { gfx::Spinner::VERT, ut::colors::burlywood }
{}

void BoardStack::pushCard(CardRef ref)
{
    assert(!ref.isNil());

    ref->move2(m_bounds.pos());
    TANK.elevateCard(ref.id);
    pushNgCard(ref->ng);
}

CardRef BoardStack::popCard()
{
    Card c;
    c.layout = layout::CardLayout::fromRect(m_bounds);
    c.assets = Card::Assets::fromNgCard(popNgCard());
    return TANK.addCard(c).ref();
}

void BoardStack::pushNgCard(ng::Card const& card)
{
    m_card_ngs.push_back(card);
    m_spinner.anim(m_card_ngs.size());
}

ng::Card BoardStack::popNgCard()
{
    assert(!m_card_ngs.empty());

    auto card = m_card_ngs.back();
    m_card_ngs.pop_back();
    m_spinner.anim(m_card_ngs.size());
    return card;
}

void BoardStack::setTestCards(size_t n)
{
    m_card_ngs.clear();
    for (size_t i = 0; i < n; ++i)
    {
        auto id = (ng::CardID)RNG.nexti((int)ng::CardID::CARD_COUNT_);
        m_card_ngs.push_back(ng::getCard(id));
    }

    m_spinner.anim(m_card_ngs.size());
}

void BoardStack::setCards(ng::cardlist_t cards)
{
    m_card_ngs = std::move(cards);
    m_spinner.anim(m_card_ngs.size());
}

void BoardStack::layout(ut::rect const& b)
{
    m_bounds = layout::CardLayout::fromRect(b).getRect();
    m_spinner.layout(b.anchorBCtoBC(b.size()/3));
}

void BoardStack::update(float dt)
{
    m_spinner.update(dt);
}

void BoardStack::drawAboveCards()
{
    auto r = m_bounds;
    auto t = m_card_back;
    auto o = 2.5f;
    drawTexture(t, r.withOffset({ o, o}));
    drawTexture(t, r.withOffset({-o,-o}));
    drawTexture(t, r);

    gfx::drawRectangleGradientEx(r,
                                 ut::colors::transparent,
                                 ut::colors::black.withNormalA(0.5f),
                                 ut::colors::black.withNormalA(0.5f),
                                 ut::colors::transparent);

    m_spinner.draw();

    DRECT_PUSH2(BoardStack, m_bounds);

    DRECT_POP();
}

void BoardStack::drawUnderCards()
{

}

void BoardStack::drawDebug()
{

}