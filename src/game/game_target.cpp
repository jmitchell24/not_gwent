//
// Created by james on 12/1/23.
//

#include "game/game_target.hpp"
#include "game/game_player.hpp"
#include "game/game_cast.hpp"
using namespace game;
using namespace game::visitors;

//
// ChangeTarget
//

void ChangeTarget::operator()(TargetDefault const& t)
{
    player.clearAllHighlights();
}

void ChangeTarget::operator()(TargetCastUnit const& t)
{
    player.clearAllHighlights();

    if (t.target_opponent)
    {
        if (t.target_melee) opponent.melee.units.setRowHighlight();
        if (t.target_ranged) opponent.ranged.units.setRowHighlight();
        if (t.target_siege) opponent.siege.units.setRowHighlight();
    }
    else
    {
        if (t.target_melee) player.melee.units.setRowHighlight();
        if (t.target_ranged) player.ranged.units.setRowHighlight();
        if (t.target_siege) player.siege.units.setRowHighlight();
    }


}

void ChangeTarget::operator()(TargetScorch const& t)
{
    player.clearAllHighlights();

    player.melee.units.setRowHighlight();
    player.ranged.units.setRowHighlight();
    player.siege.units.setRowHighlight();
}

void ChangeTarget::operator()(TargetDecoy const& t)
{
    player.clearAllHighlights();


}

void ChangeTarget::operator()(TargetBuff const& t)
{
    player.clearAllHighlights();

    player.melee.units.setRowHighlight();
    player.ranged.units.setRowHighlight();
    player.siege.units.setRowHighlight();
}

void ChangeTarget::operator()(TargetNerf const& t)
{
    player.clearAllHighlights();

    if (t.target_melee) player.melee.units.setRowHighlight();
    if (t.target_ranged) player.ranged.units.setRowHighlight();
    if (t.target_siege) player.siege.units.setRowHighlight();
}

//
// CastTarget
//

bool CastTarget::operator()(TargetDefault const& t)
{
    if (player.lead.isTargeted(mp))
    {
        cast = CastLeaderAbility{ };
        return true;
    }

    if (board::BoardRow::TargetedCard tc; player.hand.tryGetTargetedCard(mp, tc))
    {
        switch (tc.ref->ng.type)
        {
            case ng::CARD_SPECIAL:
            {
                auto special = tc.ref->ng.asSpecialCard();
                switch (special.type)
                {
                    case ng::SPECIAL_CMDR_HORN:
                        player.changeTarget(TargetBuff { tc.idx }, opponent);
                        return false;

                    case ng::SPECIAL_DECOY:
                        player.changeTarget(TargetDecoy { tc.idx }, opponent);
                        return false;

                    case ng::SPECIAL_SCORCH:
                        player.changeTarget(TargetScorch{ tc.idx }, opponent);
                        return false;


                    case ng::SPECIAL_BITING_FROST:
                        player.changeTarget(TargetNerf{true, false, false, true, tc.idx}, opponent);
                        return false;

                    case ng::SPECIAL_IMP_FOG:
                        player.changeTarget(TargetNerf{false, true, false, true, tc.idx}, opponent);
                        return false;

                    case ng::SPECIAL_TOR_RAIN:
                        player.changeTarget(TargetNerf{false, false, true, true, tc.idx}, opponent);
                        return false;

                    case ng::SPECIAL_SKELLIGE_STORM:
                        player.changeTarget(TargetNerf{false, true, true, true, tc.idx}, opponent);
                        return false;

                    case ng::SPECIAL_CLEAR_WEATHER:
                        player.changeTarget(TargetNerf{true, true, true, false, tc.idx}, opponent);
                        return false;

                    default:nopath_case(ng::SpecialType);
                }

                return false;
            }

            case ng::CARD_UNIT:
            {
                auto unit = tc.ref->ng.asUnitCard();
                auto is_spy = unit.ability == ng::ABILITY_SPY;

                switch (unit.row)
                {
                    case ng::ROW_MELEE        : player.changeTarget(TargetCastUnit{ true , false, false, is_spy, tc.idx }, opponent); return false;
                    case ng::ROW_RANGED       : player.changeTarget(TargetCastUnit{ false, true , false, is_spy, tc.idx }, opponent); return false;
                    case ng::ROW_MELEE_RANGED : player.changeTarget(TargetCastUnit{ true , true , false, is_spy, tc.idx }, opponent); return false;
                    case ng::ROW_SIEGE        : player.changeTarget(TargetCastUnit{ false, false, true , is_spy, tc.idx }, opponent); return false;
                    default:nopath_case(ng::RowFlag_);
                }
                return false;
            }

            default: nopath_case(ng::CardType);
        }
    }

    return false;
}

bool CastTarget::operator()(TargetCastUnit const& t)
{
    if (t.target_opponent)
        nopath_impl;

    if (t.target_melee && player.melee.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::MELEE, t.hand_idx };
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }

    if (t.target_ranged && player.ranged.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::RANGED, t.hand_idx };
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }

    if (t.target_siege && player.siege.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::SIEGE, t.hand_idx };
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }

    return false;
}

bool CastTarget::operator()(TargetScorch const& t)
{
    bool target =
        player.melee.units.isTargeted(mp)   ||
        player.ranged.units.isTargeted(mp)  ||
        player.siege.units.isTargeted(mp);

    if (target)
    {
        cast = CastScorch{t.hand_idx};
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetDecoy const& t)
{
    nopath_impl;
    return false;
}

bool CastTarget::operator()(TargetBuff const& t)
{
    if (player.melee.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::MELEE, t.hand_idx};
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }

    if (player.ranged.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::RANGED, t.hand_idx};
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }

    if (player.siege.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::SIEGE, t.hand_idx};
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetNerf const& t)
{

    bool target_melee  = t.target_melee  && player.melee.units.isTargeted(mp);
    bool target_ranged = t.target_ranged && player.ranged.units.isTargeted(mp);
    bool target_siege  = t.target_siege  && player.siege.units.isTargeted(mp);

    if (target_melee || target_ranged || target_siege)
    {
        cast = CastRowNerf{t.target_melee, t.target_ranged, t.target_siege, t.has_nerf_value, t.hand_idx};
        player.changeTarget(TargetDefault{}, opponent);
        return true;
    }
    return false;
}