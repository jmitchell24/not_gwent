//
// Created by james on 12/1/23.
//

#include "game/game_cast.hpp"
#include "game/game2.hpp"
using namespace game;
using namespace game::visitors;

//
// Helper functions
//

inline static void scorchRow(board::BoardRow& row, int max_strength)
{



}

//
// CardCaster -> Implementation
//

void CardCaster::operator() (CastUnit const& c)
{
    switch (c.row)
    {

        case CastUnit::MELEE : gb.boss.rowToRow(player.hand, player.melee.units , c.hand_idx, 0); break;
        case CastUnit::RANGED: gb.boss.rowToRow(player.hand, player.ranged.units, c.hand_idx, 0); break;
        case CastUnit::SIEGE : gb.boss.rowToRow(player.hand, player.siege.units , c.hand_idx, 0); break;
        default:nopath_case(CastUnit::Row);
    }
}

void CardCaster::operator() (CastSpy const& c)
{
    nopath_impl;
}

void CardCaster::operator() (CastRowBuff const& c)
{
    switch (c.row)
    {
        case CastRowBuff::MELEE:
            player.melee.has_buff = true;
            gb.boss.rowToStack(player.hand, player.yard, c.hand_idx);
            break;

        case CastRowBuff::RANGED:
            player.ranged.has_buff = true;
            gb.boss.rowToStack(player.hand, player.yard, c.hand_idx);
            break;

        case CastRowBuff::SIEGE:
            player.siege.has_buff = true;
            gb.boss.rowToStack(player.hand, player.yard, c.hand_idx);
            break;

        default:
            nopath_case(CastRowBuff::Row);
    }
}

void CardCaster::operator() (CastRowNerf const& c)
{
    if (c.change_melee)
    {
        player.melee.has_nerf = c.has_nerf_value;
        opponent.melee.has_nerf = c.has_nerf_value;
    }

    if (c.change_ranged)
    {
        player.ranged.has_nerf = c.has_nerf_value;
        opponent.ranged.has_nerf = c.has_nerf_value;
    }

    if (c.change_siege)
    {
        player.siege.has_nerf = c.has_nerf_value;
        opponent.siege.has_nerf = c.has_nerf_value;
    }

    gb.boss.rowToStack(player.hand, player.yard, c.hand_idx);
}

void CardCaster::operator() (CastScorch const& c)
{
    int max_strength = 0;

    if (int x = player.melee.units.getMaxStrength();    x > max_strength) { max_strength = x; }
    if (int x = player.ranged.units.getMaxStrength();   x > max_strength) { max_strength = x; }
    if (int x = player.siege.units.getMaxStrength();    x > max_strength) { max_strength = x; }

    if (int x = opponent.melee.units.getMaxStrength();  x > max_strength) { max_strength = x; }
    if (int x = opponent.ranged.units.getMaxStrength(); x > max_strength) { max_strength = x; }
    if (int x = opponent.siege.units.getMaxStrength();  x > max_strength) { max_strength = x; }

    auto p = [max_strength](CardRef ref)
    {
        int strength = (int) ref->ng.asUnitCard().strength;
        return strength == max_strength;
    };

#define SCORCH_ROW(row_, stack_) \
    gb.boss.rowToStack(row_, stack_, (row_).getCardIndicesIf(p));

    SCORCH_ROW(player.melee.units, player.yard);
    SCORCH_ROW(player.ranged.units, player.yard);
    SCORCH_ROW(player.siege.units, player.yard);

    SCORCH_ROW(opponent.melee.units, opponent.yard);
    SCORCH_ROW(opponent.ranged.units, opponent.yard);
    SCORCH_ROW(opponent.siege.units, opponent.yard);
#undef SCORCH_ROW

    gb.boss.rowToStack(player.hand, player.yard, c.hand_idx);
}

void CardCaster::operator() (CastLeaderAbility const& c)
{
    nopath_impl;
}
