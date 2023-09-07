//
// Created by james on 9/7/23.
//

#include "game/conv.hpp"
#include "game/card_mover.hpp"
using namespace game;

//
// gfx
//
using namespace gfx;

//
// ut
//
using namespace ut;

//
// CardSpring -> Implementation
//

void CardMover::CardSpring::update()
{
    auto mp = tout(GetMousePosition());

    spring.setDst(mp - offset);
    spring.update(GetFrameTime());

    card.setPosition(spring.now());
    card.update();
}

void CardMover::CardSpring::draw()
{
    card.draw();
}

//
// CardMover -> Implementation
//

void CardMover::set(cardlists_type cardlists)
{
    m_cardlists = std::move(cardlists);
    m_moving_card.reset();
}


void CardMover::update()
{
    auto mp = tout(GetMousePosition());
    for (auto&& it: m_cardlists)
        updateCL(mp, *it);

    if (m_moving_card)
    {
        m_moving_card->update();
    }
}

void CardMover::draw()
{
    if (m_moving_card)
        m_moving_card->draw();
}

void CardMover::updateCL(vec2 const& mp, CardList& cl)
{
    if (m_moving_card)
    {
        if (size_t idx; cl.layoutGhosted().tryGetIndex(mp, idx))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                auto card = m_moving_card->card;
                card.animDrop();
                cl.addCard(idx, card);
                m_moving_card.reset();
            }
            else
            {
                cl.setGhost(idx);
            }
        }
        else
        {
            cl.clearGhost();
        }
    }
    else
    {
        if (size_t idx; cl.layoutHovered().tryGetIndex(mp, idx))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                auto card       = cl.removeCard(idx);
                auto offset     = mp - card.position();
                auto spring     = gfx::SpringVec2 { 0.5f, 6.0f * PI };

                card.animGrab();
                spring.setNow(mp - offset);

                m_moving_card = { card, offset, spring };

                cl.setGhost(idx);
            }
            else
            {
                cl.setHover(idx);
            }
        }
        else
        {
            cl.clearHover();
        }
    }
}