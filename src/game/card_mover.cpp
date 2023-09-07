//
// Created by james on 9/7/23.
//

#include "game/conv.hpp"
#include "game/card_mover.hpp"
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

void CardMover::set(containerlist_type containers)
{
    m_containers = std::move(containers);
    m_moving_card.reset();
}


void CardMover::update()
{
    auto mp = tout(GetMousePosition());
    for (auto&& it: m_containers)
        updateContainer(mp, *it);

    if (m_moving_card)
    {
        m_moving_card->update();
    }
}

void CardMover::draw()
{
    if (m_moving_card)
        m_moving_card->draw();

    for (auto&& it : m_containers)
    {
        if (it->hasGhost())
        {
            auto r = it->getGhostRect();
            auto c = colors::red;

            drawRect(r, c.withNormalA(0.15));
            drawRectOutline(r, 2.0f, c.withNormalA(0.50));
        }

        if (it->hasHover())
        {
            auto r = it->getHoverRect();
            auto c = colors::greenyellow;

            drawRect(r, c.withNormalA(0.15));
            drawRectOutline(r, 2.0f, c.withNormalA(0.50));
        }
    }
}

void CardMover::updateContainer(vec2 const& mp, CardContainer& cl)
{
    if (m_moving_card)
    {
        if (size_t idx; cl.tryGetGhostIndex(mp, idx))
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
        if (size_t idx; cl.tryGetHoveredIndex(mp, idx))
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                auto card       = cl.removeCard(idx);
                auto offset     = mp - card.position();
                auto spring     = gfx::SpringVec2 { 0.40f, 6.0f * PI };

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