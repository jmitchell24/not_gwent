#include "game/board/board_slot.hpp"
#include "game/layout/card_layout.hpp"
using namespace game::board;
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
    Card c;
    c.ng         = ng;
    c.assets     = Card::Assets::fromNgCard(c.ng);
    c.layout     = layout::CardLayout::fromRect(m_bounds_card);

    m_card = TANK.addCard(c).ref();
}

void BoardSlot::setHighlight()
{
    m_highlighted = true;
}

void BoardSlot::clearHighlight()
{
    m_highlighted = false;
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

void BoardSlot::drawAboveCards()
{
    if (m_highlighted)
    {
        drawRectOutline(m_bounds_card, 2.0f, ut::colors::red);
    }
}
void BoardSlot::drawUnderCards() { }
void BoardSlot::drawDebug     () { }

bool BoardSlot::isTargeted(vec2 const& mp) const
{
    return m_bounds.contains(mp);
}


