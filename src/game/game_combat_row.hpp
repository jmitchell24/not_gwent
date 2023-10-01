//
// Created by james on 9/27/23.
//

#pragma once

#include "game/layout/board_layout.hpp"

#include "game/board/board_slot.hpp"
#include "game/board/board_row.hpp"

#include "gfx/gfx_spinner.hpp"

namespace game
{
    class CombatRow
    {
    public:
        board::BoardSlot    cmdr_horn;
        board::BoardSlot    weather;
        board::BoardRow     units;

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);
        int updateScore();

        void layout(layout::CombatRow const& l);
        void update(float dt);
        void drawAboveCards();
        void drawUnderCards();

        void clearHighlight()
        {
            cmdr_horn.clearHighlight();
            weather.clearHighlight();
            units.clearHighlight();
        }

    private:
        gfx::Spinner m_score {gfx::Spinner::HORZ};

    };
}