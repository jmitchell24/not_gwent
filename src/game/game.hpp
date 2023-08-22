#pragma once

#include "assert.hpp"
#include "record.hpp"

#include "game/conv.hpp"
#include "game/card.hpp"
#include "game/cardlist.hpp"

#include "game/object.hpp"
#include "gfx/gfx_easings.hpp"

//
// raylib
//
#include "raylib.h"

//
// ut
//

#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

//
// std
//
#include <vector>
#include <optional>


namespace game
{
    class CardMover
    {
    public:
        using cardlists_type    = std::vector<CardList*>;
        using movingcard_type   = std::optional<Card>;



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
                auto sz2 = m_moving_card->size()/2;
                m_moving_card->targetPosition(mp-sz2);
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

        std::optional<ut::vec2> m_dead_mp;

        void updateCL(ut::vec2 const& mp, CardList& cl)
        {
            if (m_dead_mp)
            {
                auto radius = (mp - *m_dead_mp).length();
                if (radius < 10)
                    return;
                m_dead_mp.reset();
            }

            if (m_moving_card)
            {
                if (size_t idx; cl.tryGetGhostIndex(mp, idx))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        cl.addCard(idx, *m_moving_card, CardList::ANIM_MOVE);
                        m_moving_card.reset();
                        m_dead_mp = mp;
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
                if (size_t idx; cl.tryGetHoverIndex(mp, idx))
                {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        m_moving_card = cl.removeCard(idx);
                        m_moving_card->animRaise();
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



    class CombatRow
    {
    public:
        CardList units { CardList::DIR_CENTER, Card::createTestCards(3) };
        CardList special { CardList::DIR_CENTER, Card::createTestCards(1) };

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        unsigned m_score;
        ut::rect m_bounds;
        ut::rect m_bounds_score;


#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };



    class PlayerStats
    {
    public:
        enum Gems { GEMS_NONE, GEMS_ONE, GEMS_TWO };

        ut::cstrview    m_name         = "Geralt";
        ut::cstrview    m_deckname     = "Northern Realms";

        Gems            m_gems         = GEMS_NONE;
        unsigned        m_handcount    = 3;
        unsigned        m_score        = 32;

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        ut::rect m_bounds;
        ut::rect m_bounds_name;
        ut::rect m_bounds_deckname;
        ut::rect m_bounds_gems;
        ut::rect m_bounds_gems_name;
        ut::rect m_bounds_handcount;
        ut::rect m_bounds_handcount_name;
        ut::rect m_bounds_score;
        ut::rect m_bounds_score_name;

        ut::rect m_bounds_avatar;
        ut::rect m_bounds_leader;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };

    class GameBoard
    {
    public:
        // enum Layout { LAYOUT_LANDSCAPE, LAYOUT_PORTRAIT };

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        ut::rect m_bounds;


        CombatRow m_combat_cpu_siege;
        CombatRow m_combat_cpu_ranged;
        CombatRow m_combat_cpu_melee;

        CombatRow m_combat_usr_siege;
        CombatRow m_combat_usr_ranged;
        CombatRow m_combat_usr_melee;

        CardList m_hand_usr { CardList::DIR_LEFT, Card::createTestCards(3) };
        CardList m_hand_cpu { CardList::DIR_LEFT, Card::createTestCards(3) };

        PlayerStats m_stats_cpu;
        PlayerStats m_stats_usr;

        CardMover m_card_mover;

        // hand




#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}
