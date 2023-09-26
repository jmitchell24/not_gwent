#include "game/board/board_slot.hpp"
#include "game/layout/card_layout.hpp"
using namespace game::board;
using namespace game;

//
// ut
//
using namespace ut;

bool BoardSlot::tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
{
    if (m_card && m_bounds.contains(mp))
    {
        ref = m_card;
        return true;
    }
    return false;
}

void BoardSlot::takeCard(CardRef ref)
{
    assert(m_card.isNil());
    assert(ref.inTank());

    m_card = ref;
    m_card->move2(m_bounds_card.pos());
}

void BoardSlot::setCard(ng::Card const& ng)
{
    Card card;
    card.ng         = ng;
    card.assets     = Card::Assets::fromNgCard(card.ng);
    card.layout     = layout::CardLayout::fromRect(m_bounds_card);

    m_card = TANK.addCard(card).ref();
}

CardRef BoardSlot::giveCard()
{
    assert(m_card);
    CardRef ref = m_card;
    m_card.reset();
    return ref;
}

void BoardSlot::layout(rect const& b)
{
    m_bounds = b;
    m_bounds_card = layout::CardLayout::rectFromRect(m_bounds);
}

void BoardSlot::drawAboveCards() { }
void BoardSlot::drawUnderCards() { }
void BoardSlot::drawDebug     () { }


