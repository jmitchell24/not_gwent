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
        game_layout::PlayerRow const& row_player,
        game_layout::CombatRow const& row_melee,
        game_layout::CombatRow const& row_ranged,
        game_layout::CombatRow const& row_siege,
        game_layout::StatsBoard const& player_stats)
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

bool Player::tryGetHoveredCard(vec2 const& mp, CardRef& ref)
{
    return
        hand  .tryGetHoveredCard(mp, ref) ||
        melee .tryGetHoveredCard(mp, ref) ||
        ranged.tryGetHoveredCard(mp, ref) ||
        siege .tryGetHoveredCard(mp, ref) ||
        lead  .tryGetHoveredCard(mp, ref);
}

//board::BoardStack& Player::getStack(StackType x)
//{
//    switch (x)
//    {
//        case STACK_DECK: return deck;
//        case STACK_YARD: return yard;
//        default:nopath_case(StackType);
//    }
//
//    nopath_impl;
//}
//
//board::BoardRow& Player::getRow(RowType x)
//{
//    switch (x)
//    {
//        case ROW_MELEE: return melee.units;
//        case ROW_RANGED: return ranged.units;
//        case ROW_SIEGE: return siege.units;
//        default: nopath_case(RowType);
//    }
//    nopath_impl;
//}
//
//board::BoardSlot& Player::getSlot(SlotType x)
//{
//    switch (x)
//    {
//
//    }
//    nopath_impl;
//}

void Player::changeTarget(CastTargetParams& p, Target t)
{
    target = t;
    std::visit(visitors::ChangeTarget{p}, target);
}

bool Player::tryCast(CastTargetParams p, Cast& cast, vec2 const& mp)
{
    return std::visit(visitors::CastTarget{p, cast, mp}, target);
}

void Player::cancelCast(CastTargetParams p)
{
    changeTarget(p, TargetDefault{});

//    if (moveto_uncast.index() != 0)
//    {
//        auto& uc = std::get<MoveToBoardRow>(moveto_uncast);
//
//        hand_cast.removeCard(uc.idx);
//        hand.addCard(0, uc.ref);
//        hand_cast.clearCardHighlight();
//        moveto_uncast = MoveToBoardRow { ROW_HAND, uc.idx, uc.ref };
//        TANK.moveCard(uc.ref.id, CARD_LAYER_BOARD);
//    }

}

void Player::update(update_param u)
{
    float dt = u.frame_time;
    deck.update(dt);
    yard.update(dt);
    hand.update(u);
    stats.update(dt);

    melee.update(dt);
    ranged.update(dt);
    siege.update(dt);
}

void Player::drawAboveCards()
{
    deck .drawAboveCards();
    yard .drawAboveCards();
    hand .drawAboveCards();
    stats.drawAboveCards();

    melee .drawAboveCards();
    ranged.drawAboveCards();
    siege .drawAboveCards();
}

void Player::drawUnderCards()
{
    deck .drawUnderCards();
    yard .drawUnderCards();
    hand .drawUnderCards();
    stats.drawUnderCards();

    melee .drawUnderCards();
    ranged.drawUnderCards();
    siege .drawUnderCards();
}

void Player::drawDebug()
{
    deck.drawDebug();
    yard.drawDebug();
    hand.drawDebug();
    stats.drawDebug();
}

