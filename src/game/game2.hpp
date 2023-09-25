//
// Created by james on 9/15/23.
//

#pragma once

#include "game/board/board_row.hpp"
#include "game/board/board_stack.hpp"
#include "game/board/board_slot.hpp"
#include "game/board/board_boss.hpp"
#include "game/board/board_stats.hpp"

#include "game/layout/board_layout.hpp"

namespace game
{
    struct GameBoard2
    {
        struct CombatRow
        {
            gfx::Spinner        score;
            board::BoardSlot    special;
            board::BoardRow     units;

            void layout(layout::CombatRow const& l)
            {
                special.layout(l.special);
                units.layout(l.units);
                score.layout(l.score);
            }

            bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
            {
                return
                    special.tryGetHoveredCard(mp, ref) ||
                    units  .tryGetHoveredCard(mp, ref);
            }
        };

        struct Player
        {
            board::BoardStack  deck{board::BoardStack::DECK};
            board::BoardStack  yard{board::BoardStack::YARD};
            board::BoardSlot   lead;
            board::BoardRow    hand;
            board::BoardStats  stats;

            CombatRow melee;
            CombatRow ranged;
            CombatRow siege;

            void layout(
                    layout::PlayerRow const& row_player,
                    layout::CombatRow const& row_melee,
                    layout::CombatRow const& row_ranged,
                    layout::CombatRow const& row_siege,
                    layout::StatsBoard const& player_stats)
            {
                deck.layout(row_player.deck);
                yard.layout(row_player.yard);
                hand.layout(row_player.hand);
                lead.layout(row_player.lead);
                stats.layout(player_stats);

                melee .layout(row_melee);
                ranged.layout(row_ranged);
                siege .layout(row_siege);
            }

            bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
            {
                return
                    hand  .tryGetHoveredCard(mp, ref) ||
                    melee .tryGetHoveredCard(mp, ref) ||
                    ranged.tryGetHoveredCard(mp, ref) ||
                    siege .tryGetHoveredCard(mp, ref);
            }

            void update(float dt)
            {
                deck.update(dt);
                yard.update(dt);
                hand.update(dt);
                stats.update(dt);

            }

            void drawAboveCards()
            {
                deck.drawAboveCards();
                yard.drawAboveCards();
                hand.drawAboveCards();
                stats.drawAboveCards();
            }

            void drawUnderCards()
            {
                deck.drawUnderCards();
                yard.drawUnderCards();
                hand.drawUnderCards();
                stats.drawUnderCards();
            }

            void drawDebug()
            {
                deck.drawDebug();
                yard.drawDebug();
                hand.drawDebug();
                stats.drawDebug();
            }
        } usr, cpu;



        CardRef card_hover;

        layout::GameBoard gb;
        board::BoardBoss boss;


        void layout(ut::rect const& bounds);
        void update(float dt);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug     ();

        //
        // actions
        //

        void setHoveredCard(CardRef ref)
        {
            if (!card_hover.isNil())
            {

            }
        }
    };
}