#pragma once

#include "assert.hpp"
#include "record.hpp"

#include "game/conv.hpp"
#include "game/card.hpp"
#include "game/cardlist.hpp"

#include "game/object.hpp"
#include "gfx/gfx_animation.hpp"

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
        using cardlists_type = std::vector<CardList*>;
        using movingcard_type = std::optional<Card>;



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
        std::vector<CardList*>  m_cardlists;
        std::optional<Card>     m_moving_card;

        void updateCL(ut::vec2 const& mp, CardList& cl)
        {
            if (m_moving_card)
            {
                if (size_t idx; cl.tryGetGhostIndex(mp, idx))
                {


                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        cl.addCard(idx, *m_moving_card, CardList::ANIM_MOVE);
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
                if (size_t idx; cl.tryGetHoverIndex(mp, idx))
                {


                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        m_moving_card =  cl.removeCard(idx);
                        m_moving_card->targetElevation(2.0f);
                        m_moving_card->targetOpacity(0.5f);
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
        CardList units {CardList::DIR_CENTER, Card::createTestCards(3) };
        CardList special { CardList::DIR_CENTER, Card::createTestCards(1) };

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:

        ut::rect m_bounds;


#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };



    class StatDisplay : public Layout
    {
    public:
        StatDisplay(ut::cstrparam player_name);

        ut::cstrview playerName() const { return m_player_name; }

        unsigned gems           () const { return 0; }
        unsigned score          () const { return 0; }
        unsigned handSize       () const { return 0; }
        unsigned librarySize    () const { return 0; }
        unsigned graveyardSize  () const { return 0; }

        void load();
        void unload();
        void update();
        void draw();

    protected:
        void onLayout(ut::rect const& bounds) override;

    private:
        std::array<ut::rect, 5> m_bounds_stats;

        ut::cstrview    m_player_name;

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

        CardMover m_card_mover;

        // hand




#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}
