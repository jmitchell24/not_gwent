//
// Created by james on 12/13/23.
//

#include "game/game_card_picker.hpp"
using namespace game;


bool CardPicker::wantCaptureMouse() const
{
    return m_open;
}

void CardPicker::open(cardrefs_param refs)
{
    m_open = true;
    row.addCardMulti(0, refs);
}

void CardPicker::close()
{
    m_open = false;
}

void CardPicker::layout(layout::CardPicker const& cp)
{
    r_button_cancel = cp.cancel;
    r_button_confirm = cp.confirm;
    r_title = cp.title;
}

void CardPicker::update(update_param u)
{
    if (m_open) row.update(u.frame_time);
}

void CardPicker::drawAboveCards()
{
    if (m_open) row.drawAboveCards();
}
void CardPicker::drawUnderCards()
{
    if (m_open) row.drawUnderCards();
}
void CardPicker::drawDebug()
{
    if (m_open) row.drawDebug();
}