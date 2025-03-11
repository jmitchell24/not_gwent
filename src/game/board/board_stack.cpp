#include "game/board/board_stack.hpp"
#include "res/res.hpp"
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
        case BoardStack::YARD: return res::textures::card_back_neutral();
        case BoardStack::DECK: return res::textures::card_back_graveyard();
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

    ref->animBounds(m_bounds);
    TANK.elevateCard(ref.id);
    pushNgCard(ref->ng);
}

CardRef BoardStack::popCard()
{
    Card c;
    c.ng         = popNgCard();
    c.assets     = Card::Assets::fromNgCard(c.ng);
    c.layout     = CardLayout::fromRect(m_bounds);
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

card_ref_list_t BoardStack::getUnitCards(CardLayer layer)
{
    card_ref_list_t refs;
    for (auto&& it : m_card_ngs)
    {
        if (it.isUnitCard())
        {
            Card c;
            c.ng         = it;
            c.assets     = Card::Assets::fromNgCard(c.ng);
            c.layout     = CardLayout::fromRect(m_bounds);

            refs.push_back(TANK.addCard(c, layer).ref());
        }
    }
    return refs;
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
    m_bounds = CardLayout::fromRect(b).getRect();
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
}

void BoardStack::drawUnderCards()
{

}

void BoardStack::drawDebug()
{

}

//
// Implementation -> CardRefListPile
//

void CardRefListPile::layout(rect const& bounds)
{
    m_bounds = CardLayout::fromRect(bounds).getRect();
    m_spinner.layout(bounds.anchorBCtoBC(bounds.size()/3));
}

void CardRefListPile::update(float dt)
{
    m_spinner.update(dt);
}

void CardRefListPile::drawAboveCards()
{
    gfx::drawRectOutline(m_bounds, 2.0f, colors::purple);
    m_spinner.draw();
}

void CardRefListPile::drawUnderCards()
{

}

void CardRefListPile::drawDebug()
{

}

void CardRefListPile::onContainerChanged()
{

}