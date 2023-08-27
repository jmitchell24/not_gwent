//
// Created by james on 8/26/23.
//

#pragma once

#include "game/card.hpp"
#include "gfx/gfx_spring.hpp"

namespace game
{
    class CardMover
    {
    public:
        struct CardSpring
        {
            Card card;
            ut::vec2 offset;
            gfx::SpringVec2 spring;

            void update()
            {
                auto mp = tout(GetMousePosition());

                spring.setDst(mp - offset);
                spring.update(GetFrameTime());

                card.setPosition(spring.now());
                card.update();
            }

            void draw()
            {
                card.draw();
            }
        };

        using cardlists_type    = std::vector<CardList*>;
        using movingcard_type   = std::optional<CardSpring>;



        void set(cardlists_type cardlists)
        {
            m_cardlists = std::move(cardlists);
            m_moving_card.reset();
        }


        void update()
        {
            auto mp = tout(GetMousePosition());
            for (auto&& it: m_cardlists)
                updateCL(mp, *it);

            if (m_moving_card)
            {
                m_moving_card->update();
            }
        }

        void draw()
        {
            if (m_moving_card)
                m_moving_card->draw();
        }



    private:
        cardlists_type      m_cardlists;
        movingcard_type     m_moving_card;

        void updateCL(ut::vec2 const& mp, CardList& cl)
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
    };
}
