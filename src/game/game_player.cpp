//
// Created by james on 9/29/23.
//

#include "game/game_player.hpp"
using namespace game;


//
// ut
//
#include <ut/check.hpp>
using namespace ut;

//
// Player -> Implementation
//

void Player::layout(
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

bool Player::tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
{
    return
        hand  .tryGetHoveredCard(mp, ref) ||
        melee .tryGetHoveredCard(mp, ref) ||
        ranged.tryGetHoveredCard(mp, ref) ||
        siege .tryGetHoveredCard(mp, ref) ||
        lead  .tryGetHoveredCard(mp, ref);
}

void Player::changeTarget(Target t)
{
    target = t;
    std::visit(ChangePlayerTarget{*this}, target);
}

bool Player::tryCast(ut::vec2 const& mp, Cast& cast)
{
    return std::visit(CastTarget{*this, mp, cast}, target);
}

void Player::cancelCast()
{
    changeTarget(TargetDefault{});
}

void Player::update(float dt)
{
    deck.update(dt);
    yard.update(dt);
    hand.update(dt);
    stats.update(dt);

    melee.update(dt);
    ranged.update(dt);
    siege.update(dt);
}

void Player::drawAboveCards()
{
    deck.drawAboveCards();
    yard.drawAboveCards();
    hand.drawAboveCards();
    stats.drawAboveCards();

    melee.drawAboveCards();
    ranged.drawAboveCards();
    siege.drawAboveCards();
}

void Player::drawUnderCards()
{
    deck.drawUnderCards();
    yard.drawUnderCards();
    hand.drawUnderCards();
    stats.drawUnderCards();

    melee.drawUnderCards();
    ranged.drawUnderCards();
    siege.drawUnderCards();
}

void Player::drawDebug()
{
    deck.drawDebug();
    yard.drawDebug();
    hand.drawDebug();
    stats.drawDebug();
}

//
// ChangePlayerTarget
//

void ChangePlayerTarget::operator()(TargetDefault const& t)
{
    player.clearAllHighlights();
}

void ChangePlayerTarget::operator()(TargetUnitRow const& t)
{
    player.clearAllHighlights();

    if (t.target_opponent)
        nopath_impl;

    if (t.target_melee) player.melee.units.setHighlight();
    if (t.target_ranged) player.ranged.units.setHighlight();
    if (t.target_siege) player.siege.units.setHighlight();
}

void ChangePlayerTarget::operator()(TargetUnitCard const& t)
{
    player.clearAllHighlights();
    nopath_impl;
}

void ChangePlayerTarget::operator()(TargetBuff const& t)
{
    player.clearAllHighlights();

    player.melee.units.setHighlight();
    player.ranged.units.setHighlight();
    player.siege.units.setHighlight();
}

void ChangePlayerTarget::operator()(TargetNerf const& t)
{
    player.clearAllHighlights();

    if (t.target_melee) player.melee.units.setHighlight();
    if (t.target_ranged) player.ranged.units.setHighlight();
    if (t.target_siege) player.siege.units.setHighlight();
}

//
// CastTarget
//

bool CastTarget::operator()(TargetUnitRow const& t)
{
    if (t.target_opponent)
        nopath_impl;

    if (t.target_melee && player.melee.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::MELEE, t.hand_idx };
        player.changeTarget(TargetDefault{});
        return true;
    }

    if (t.target_ranged && player.ranged.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::RANGED, t.hand_idx };
        player.changeTarget(TargetDefault{});
        return true;
    }

    if (t.target_siege && player.siege.units.isTargeted(mp))
    {
        cast = CastUnit { CastUnit::SIEGE, t.hand_idx };
        player.changeTarget(TargetDefault{});
        return true;
    }

    return false;
}

bool CastTarget::operator()(TargetUnitCard const& t)
{
    nopath_impl;
    return false;
}

bool CastTarget::operator()(TargetBuff const& t)
{
    if (player.melee.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::MELEE, t.hand_idx};
        player.changeTarget(TargetDefault{});
        return true;
    }

    if (player.ranged.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::RANGED, t.hand_idx};
        player.changeTarget(TargetDefault{});
        return true;
    }

    if (player.siege.units.isTargeted(mp))
    {
        cast = CastRowBuff{CastRowBuff::SIEGE, t.hand_idx};
        player.changeTarget(TargetDefault{});
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
        player.changeTarget(TargetDefault{});
        return true;
    }
    return false;
}

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
                        player.changeTarget(TargetBuff { tc.idx });
                        return false;

                    case ng::SPECIAL_DECOY:
                        player.changeTarget(TargetUnitCard { tc.idx });
                        return false;

                    case ng::SPECIAL_SCORCH:
                        nopath_impl;
                        return false;


                    case ng::SPECIAL_BITING_FROST:
                        player.changeTarget(TargetNerf{true, false, false, true, tc.idx});
                        return false;

                    case ng::SPECIAL_IMP_FOG:
                        player.changeTarget(TargetNerf{false, true, false, true, tc.idx});
                        return false;

                    case ng::SPECIAL_TOR_RAIN:
                        player.changeTarget(TargetNerf{false, false, true, true, tc.idx});
                        return false;

                    case ng::SPECIAL_SKELLIGE_STORM:
                        player.changeTarget(TargetNerf{false, true, true, true, tc.idx});
                        return false;

                    case ng::SPECIAL_CLEAR_WEATHER:
                        player.changeTarget(TargetNerf{true, true, true, false, tc.idx});
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
                    case ng::ROW_MELEE        : player.changeTarget(TargetUnitRow{ true , false, false, is_spy, tc.idx }); return false;
                    case ng::ROW_RANGED       : player.changeTarget(TargetUnitRow{ false, true , false, is_spy, tc.idx }); return false;
                    case ng::ROW_MELEE_RANGED : player.changeTarget(TargetUnitRow{ true , true , false, is_spy, tc.idx }); return false;
                    case ng::ROW_SIEGE        : player.changeTarget(TargetUnitRow{ false, false, true , is_spy, tc.idx }); return false;
                    default:nopath_case(ng::RowFlag_);
                }
                return false;
            }

            default: nopath_case(ng::CardType);
        }
    }

    return false;
}