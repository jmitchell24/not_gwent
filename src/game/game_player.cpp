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

void Player::changeTarget(Target t, Player& opponent)
{
    target = t;
    std::visit(visitors::ChangeTarget{*this, opponent}, target);
}

bool Player::tryCast(ut::vec2 const& mp, Player& opponent, Cast& cast)
{
    return std::visit(visitors::CastTarget{*this, opponent, cast, mp}, target);
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

