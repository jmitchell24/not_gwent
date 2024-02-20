//
// Created by james on 9/29/23.
//

#pragma once

#include "game/game_stats.hpp"
#include "game/game_target.hpp"
#include "game/game_move.hpp"
#include "game/game_enums.hpp"
#include "game/game_combat_row.hpp"
#include "game/game_cast_list.hpp"
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
                game_layout::PlayerRow const& row_player,
                game_layout::CombatRow const& row_melee,
                game_layout::CombatRow const& row_ranged,
                game_layout::CombatRow const& row_siege,
                game_layout::StatsBoard const& player_stats);

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);

//        board::BoardStack& getStack(StackType x);
//        board::BoardRow& getRow(RowType x);
//        board::BoardSlot& getSlot(SlotType x);

        void clearAllHighlights()
        {
            melee.clearHighlight();
            ranged.clearHighlight();
            siege.clearHighlight();
            hand.clearCardHighlight();
        }

        void changeTarget(CastTargetParams& p, Target t);
        bool tryCast(CastTargetParams p, Cast& cast, ut::vec2 const& mp);

        void cancelCast(CastTargetParams p);

        void update(update_param u);
        void drawUnderCards();
        void drawAboveCards();
        void drawDebug();

    private:

    };



}