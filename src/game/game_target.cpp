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
    params.player.clearAllHighlights();
}

void ChangeTarget::operator()(TargetCastUnit const& t)
{
    params.player.clearAllHighlights();

    if (t.target_opponent)
    {
        if (t.target_melee) params.opponent.melee.units.setHighlight();
        if (t.target_ranged) params.opponent.ranged.units.setHighlight();
        if (t.target_siege) params.opponent.siege.units.setHighlight();
    }
    else
    {
        if (t.target_melee) params.player.melee.units.setHighlight();
        if (t.target_ranged) params.player.ranged.units.setHighlight();
        if (t.target_siege) params.player.siege.units.setHighlight();
    }


}

void ChangeTarget::operator()(TargetScorch const& t)
{
    params.player.clearAllHighlights();

    params.player.melee.units.setHighlight();
    params.player.ranged.units.setHighlight();
    params.player.siege.units.setHighlight();
}

void ChangeTarget::operator()(TargetDecoy const& t)
{
    params.player.clearAllHighlights();


}

void ChangeTarget::operator()(TargetBuff const& t)
{
    params.player.clearAllHighlights();

    params.player.melee.units.setHighlight();
    params.player.ranged.units.setHighlight();
    params.player.siege.units.setHighlight();
}

void ChangeTarget::operator()(TargetNerf const& t)
{
    params.player.clearAllHighlights();

    if (t.target_melee) params.player.melee.units.setHighlight();
    if (t.target_ranged) params.player.ranged.units.setHighlight();
    if (t.target_siege) params.player.siege.units.setHighlight();
}

void ChangeTarget::operator()(TargetMedic const& t)
{
    cardrefs_t refs = params.player.yard.getUnitCards(CARD_LAYER_OVERLAY);

    if (refs.empty())
        nopath_impl;

    params.card_picker.open(refs);
}

//
// CastTarget
//

bool CastTarget::operator()(TargetDefault const& t)
{
    if (params.player.lead.isTargeted(mp))
    {
        cast = CastLeaderAbility{ };
        return true;
    }

    if (board::BoardRow::TargetedCard tc; params.player.hand.tryGetTargetedCard(mp, tc))
    {
        switch (tc.ref->ng.type)
        {
            case ng::CARD_SPECIAL:
            {
                auto special = tc.ref->ng.asSpecialCard();
                switch (special.type)
                {
                    case ng::SPECIAL_CMDR_HORN:
                        setTarget(TargetBuff { tc.idx });
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    case ng::SPECIAL_SCORCH:
                        setTarget(TargetScorch{ tc.idx });
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;


                    case ng::SPECIAL_BITING_FROST:
                        setTarget(TargetNerf{true, false, false, true, tc.idx});
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    case ng::SPECIAL_IMP_FOG:
                        setTarget(TargetNerf{false, true, false, true, tc.idx});
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    case ng::SPECIAL_TOR_RAIN:
                        setTarget(TargetNerf{false, false, true, true, tc.idx});
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    case ng::SPECIAL_SKELLIGE_STORM:
                        setTarget(TargetNerf{false, true, true, true, tc.idx});
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    case ng::SPECIAL_CLEAR_WEATHER:
                        setTarget(TargetNerf{true, true, true, false, tc.idx});
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;

                    default:nopath_case(ng::SpecialType);
                }

                return false;
            }

            case ng::CARD_UNIT:
            {
                auto unit = tc.ref->ng.asUnitCard();
                auto is_spy = unit.ability == ng::ABILITY_SPY;
                auto is_decoy = unit.ability == ng::ABILITY_DECOY;
                auto is_medic = unit.ability == ng::ABILITY_MEDIC;

                switch (unit.ability)
                {
                    case ng::ABILITY_SPY:
                        nopath_impl;
                        return false;

                    case ng::ABILITY_DECOY:
                        setTarget(TargetDecoy{ tc.idx });
                        params.player.hand.setCardHighlight(tc.idx);
                        return false;
                }

                switch (unit.row)
                {
                    case ng::ROW_MELEE        : setTarget(TargetCastUnit{ true , false, false, is_spy, is_medic, tc.idx }); break;
                    case ng::ROW_RANGED       : setTarget(TargetCastUnit{ false, true , false, is_spy, is_medic, tc.idx }); break;
                    case ng::ROW_MELEE_RANGED : setTarget(TargetCastUnit{ true , true , false, is_spy, is_medic, tc.idx }); break;
                    case ng::ROW_SIEGE        : setTarget(TargetCastUnit{ false, false, true , is_spy, is_medic, tc.idx }); break;
                    default:nopath_case(ng::RowFlag_);
                }
                params.player.hand.setCardHighlight(tc.idx);
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

    if (t.target_melee && params.player.melee.units.isTargeted(mp))
    {
        if (t.is_medic)
        {
            setTarget(TargetMedic{t.hand_idx});
            return false;
        }

        setCast(CastUnit { CastUnit::MELEE, t.hand_idx });
        return true;
    }

    if (t.target_ranged && params.player.ranged.units.isTargeted(mp))
    {
        if (t.is_medic)
        {
            setTarget(TargetMedic{t.hand_idx});
            return false;
        }

        setCast(CastUnit { CastUnit::RANGED, t.hand_idx });
        return true;
    }

    if (t.target_siege && params.player.siege.units.isTargeted(mp))
    {
        if (t.is_medic)
        {
            setTarget(TargetMedic{t.hand_idx});
            return false;
        }

        setCast(CastUnit { CastUnit::SIEGE, t.hand_idx });
        return true;
    }

    return false;
}

bool CastTarget::operator()(TargetScorch const& t)
{
    bool target =
        params.player.melee.units.isTargeted(mp)   ||
        params.player.ranged.units.isTargeted(mp)  ||
        params.player.siege.units.isTargeted(mp);

    if (target)
    {
        setCast(CastScorch{t.hand_idx});
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetDecoy const& t)
{
    if (size_t idx; params.player.melee.units.tryGetHoveredIndex(mp, idx))
    {
        setCast(CastDecoy{CastDecoy::MELEE, t.hand_idx, idx});
        return true;
    }

    if (size_t idx; params.player.ranged.units.tryGetHoveredIndex(mp, idx))
    {
        setCast(CastDecoy{CastDecoy::RANGED, t.hand_idx, idx});
        return true;
    }

    if (size_t idx; params.player.siege.units.tryGetHoveredIndex(mp, idx))
    {
        setCast(CastDecoy{CastDecoy::SIEGE, t.hand_idx, idx});
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetBuff const& t)
{
    if (params.player.melee.units.isTargeted(mp))
    {
        setCast(CastRowBuff{CastRowBuff::MELEE, t.hand_idx});
        return true;
    }

    if (params.player.ranged.units.isTargeted(mp))
    {
        setCast(CastRowBuff{CastRowBuff::RANGED, t.hand_idx});
        return true;
    }

    if (params.player.siege.units.isTargeted(mp))
    {
        setCast(CastRowBuff{CastRowBuff::SIEGE, t.hand_idx});
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetNerf const& t)
{

    bool target_melee  = t.target_melee  && params.player.melee.units.isTargeted(mp);
    bool target_ranged = t.target_ranged && params.player.ranged.units.isTargeted(mp);
    bool target_siege  = t.target_siege  && params.player.siege.units.isTargeted(mp);

    if (target_melee || target_ranged || target_siege)
    {
        setCast(CastRowNerf{t.target_melee, t.target_ranged, t.target_siege, t.has_nerf_value, t.hand_idx});
        return true;
    }
    return false;
}

bool CastTarget::operator()(TargetMedic const& t)
{
    check(params.card_picker.isOpen(), "card picker should be open");

    if (CardRef ref; params.card_picker.tryClose(mp, ref))
    {
        setTarget(TargetDefault{});
    }

    return false;
}

void CastTarget::setCast(Cast const& c)
{
    cast = c;
    params.player.changeTarget(params, TargetDefault{});
}

void CastTarget::setTarget(Target const& t)
{
    params.player.changeTarget(params, t);
}