#include "game/board/board_slot.hpp"
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
    m_card->move2(m_bounds.pos());
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
}

void BoardSlot::drawAboveCards() { }
void BoardSlot::drawUnderCards() { }
void BoardSlot::drawDebug     () { }