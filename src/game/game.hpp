#pragma once

#include "assert.hpp"
#include "record.hpp"

#include "game/conv.hpp"
#include "game/card.hpp"
#include "game/card_list.hpp"
#include "game/card_stack.hpp"

#include "game/object.hpp"
#include "game/card_mover.hpp"
#include "gfx/gfx_easings.hpp"
#include "gfx/gfx_spring.hpp"

//
// ng
//
#include "ng/ng_game.hpp"

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
    class CombatRow
    {
    public:
        CardList units      { CardLayout::DIR_CENTER, {} };
        CardList special    { CardLayout::DIR_CENTER, {} };

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

    class PlayerCards
    {
    public:
        CardList hand       { CardLayout::DIR_CENTER, {} };
        CardList deck       { CardLayout::DIR_LEFT, {} };
        CardList graveyard  { CardLayout::DIR_LEFT, {} };

        void layout(ut::rect const& bounds);
        void update();
        void draw();
    private:
        ut::rect m_bounds;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };

    class WeatherBoard
    {
    public:
        CardList cards{ CardLayout::DIR_LEFT, {} };

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        ut::rect m_bounds;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };

    class PlayerBoard
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

        static constexpr ut::cstrview DEBUG_LABEL = "GameBoard";
        void drawDebug(ut::cstrparam){}
    private:
        ut::rect        m_bounds;

        CombatRow       m_combat_cpu_siege;
        CombatRow       m_combat_cpu_ranged;
        CombatRow       m_combat_cpu_melee;

        CombatRow       m_combat_usr_siege;
        CombatRow       m_combat_usr_ranged;
        CombatRow       m_combat_usr_melee;

        WeatherBoard    m_weather_cards;

        PlayerCards     m_cards_usr;
        PlayerCards     m_cards_cpu;

        PlayerBoard     m_stats_cpu;
        PlayerBoard     m_stats_usr;

        CardMover       m_card_mover;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}
