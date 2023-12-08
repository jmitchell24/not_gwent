//
// Created by james on 9/29/23.
//

#pragma once

#include "game/game_stats.hpp"
#include "game/game_target.hpp"
#include "game/game_combat_row.hpp"
#include "game/board/board_hand.hpp"

namespace game
{

    class Player
    {
    public:
        board::BoardStack  deck{board::BoardStack::DECK};
        board::BoardStack  yard{board::BoardStack::YARD};
        board::BoardSlot   lead;
        board::BoardHand   hand;

        CombatRow melee;
        CombatRow ranged;
        CombatRow siege;

        Stats stats;

        Target target = TargetDefault{};

        void layout(
                layout::PlayerRow const& row_player,
                layout::CombatRow const& row_melee,
                layout::CombatRow const& row_ranged,
                layout::CombatRow const& row_siege,
                layout::StatsBoard const& player_stats);

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);



        void clearAllHighlights()
        {
            melee.clearHighlight();
            ranged.clearHighlight();
            siege.clearHighlight();
            hand.clearCardHighlight();
        }

        void changeTarget(Target t, Player& opponent);
        bool tryCast(ut::vec2 const& mp, Player& opponent, Cast& cast);

        inline void cancelCast(Player& opponent)
        { changeTarget(TargetDefault{}, opponent); }

        void update(float dt);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug();
    };



}