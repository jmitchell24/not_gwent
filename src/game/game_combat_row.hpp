//
// Created by james on 9/27/23.
//

#pragma once

#include "game/layout/board_layout.hpp"

#include "game/board/board_slot.hpp"
#include "game/board/board_units.hpp"

#include "gfx/gfx_spinner.hpp"

namespace game
{


    class CombatRow
    {
    public:
        //board::BoardSlot    cmdr_horn;
        //board::BoardSlot    weather;

        bool has_buff = false;
        bool has_nerf = false;

        board::BoardUnits units;

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);

        int updateScore();

        void layout(game_layout::CombatRow const& l);
        void update(float dt);
        void drawUnderCards();
        void drawAboveCards();

        void clearHighlight()
        {
            //cmdr_horn.clearHighlight();
            //weather.clearHighlight();
            units.clearCardHighlight();
            units.clearHighlight();
        }

    private:
        ut::rect m_rect_buff;
        ut::rect m_rect_nerf;

        gfx::Spinner m_score {gfx::Spinner::HORZ};

    };
}