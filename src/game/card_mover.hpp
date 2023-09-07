//
// Created by james on 8/26/23.
//

#pragma once

#include "game/card_container.hpp"
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

            void update();
            void draw();
        };

        using containerlist_type    = std::vector<CardContainer*>;
        using movingcard_type       = std::optional<CardSpring>;

        void set(containerlist_type containers);
        void update();
        void draw();

    private:
        containerlist_type      m_containers;
        movingcard_type         m_moving_card;

        void updateContainer(ut::vec2 const& mp, CardContainer& container);
    };

    class CardDeleter
    {
    public:
        void addCard(Card const& card)
        {
            m_cards.push_back(card);
        }

        void update()
        {
            m_swap.clear();
            for (auto&& it : m_cards)
                if (it.update())
                    m_swap.push_back(it);
            m_cards.swap(m_swap);
        }

        void draw()
        {
            for (auto&& it : m_cards)
                it.draw();
        }
    private:
        cardlist_t m_cards;
        cardlist_t m_swap;
    };
}

#if 0
//
// Created by james on 8/26/23.
//

#pragma once

#include "game/card.hpp"
#include "gfx/gfx_spring.hpp"
#include "gfx/gfx_curves.hpp"

namespace game
{
    class CardMover
    {
    public:
        struct NoneMover { };

        struct MouseMover
        {
            Card            card;
            ut::vec2        offset;
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

        struct CurveMover
        {
            Card card;

            void update()
            {

            }

            void draw()
            {

            }
        };

        union
        {
            NoneMover   u_none;
            MouseMover  u_mouse;
            CurveMover  u_curve;
        } value;

        enum MoverType { MOVER_NONE, MOVER_MOUSE, MOVER_CURVE } type;

        using cardlists_type    = std::vector<CardList*>;



        void set(cardlists_type cardlists)
        {
            m_cardlists = std::move(cardlists);
            type = MOVER_NONE;

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

#endif
