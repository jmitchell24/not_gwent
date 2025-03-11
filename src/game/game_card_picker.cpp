//
// Created by james on 12/13/23.
//

#include "game/game_card_picker.hpp"
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
using namespace ut;

bool CardPicker::isOpen() const
{
    return m_open;
}

void CardPicker::open(card_ref_list_param refs)
{
    check(!m_open, "already open");

    m_open = true;
    m_refs = refs;
    m_row_options.addCardMulti(0, m_refs);
}

bool CardPicker::tryClose(vec2 const& mp, CardRef& ref)
{
    if (!m_open)
        return true;

    if (m_layout.cancel.outer.contains(mp))
    {
        ref.reset();
        close();
        return true;
    }

    if (!m_slot_choice.isEmpty() && m_layout.confirm.outer.contains(mp))
    {
        ref = m_slot_choice.giveCard();
        close();
        return true;
    }

    if (CardRef card{}; m_row_options.tryGetHoveredCard(mp, card))
    {
        if (!m_slot_choice.isEmpty())
        {
            auto i = m_row_options.tryGetNearestIndex(m_slot_choice.getBounds().center());
            m_row_options.addCard(i, m_slot_choice.giveCard());
        }

        m_row_options.removeCard(card);
        m_slot_choice.takeCard(card);
    }

    return false;
}

void CardPicker::close()
{
    m_open = false;

    m_row_options.clear();
    m_slot_choice.clear();
    for (auto&& it : m_refs)
        TANK.removeCard(it.id);
    m_refs.clear();
}

bool CardPicker::tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
{
    return m_row_options.tryGetHoveredCard(mp, ref);
}

void CardPicker::layout()
{
    m_row_options.layout(m_layout.card_options);
    m_slot_choice.layout(m_layout.card_choice);
}

void CardPicker::update(update_param u)
{
    if (m_open) m_row_options.update(u);
}

void CardPicker::drawAboveCards()
{
    if (!m_open)
        return;

    m_row_options.drawAboveCards();

    auto& l = m_layout;

    drawRect(l.title.border, colors::ivory);
    drawRectOutline(l.title.border, 4.0f, colors::blanchedalmond);
    drawTextCCtoCC(l.title.inner, title, colors::brown);

    drawRect(l.confirm.border, colors::limegreen);
    drawRectOutline(l.confirm.border, 4.0f, colors::green);
    drawTextCCtoCC(l.confirm.inner, "Confirm", colors::white);

    drawRect(l.cancel.border, colors::orangered);
    drawRectOutline(l.cancel.border, 4.0f, colors::darkred);
    drawTextCCtoCC(l.cancel.inner, "Cancel", colors::white);


}

void CardPicker::drawUnderCards()
{
    if (!m_open)
        return;

    m_row_options.drawUnderCards();

    auto& l = m_layout;
    drawRect(l.background, colors::black.withNormalA(0.5f));

    drawRect(l.card_choice.outer, colors::ivory);
    drawRectOutline(l.card_choice.outer, 4.0f, colors::blanchedalmond);
}

void CardPicker::drawDebug()
{
    if (m_open) m_row_options.drawDebug();
}