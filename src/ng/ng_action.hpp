//
// Created by james on 9/1/23.
//

#pragma once

#include <vector>

#define NG_ENUM_CARD_LOCATION \
    CASE(Hand           , HAND          , "Hand"        ) \
    CASE(Deck           , DECK          , "Deck"        ) \
    CASE(Graveyard      , GRAVEYARD     , "Graveyard"   ) \
    CASE(CombatMelee    , COMBAT_MELEE  , "CombatMelee" ) \
    CASE(CombatRanged   , COMBAT_RANGED , "CombatRanged") \
    CASE(CombatSiege    , COMBAT_SIEGE  , "CombatSiege" )

/* TODO: Think about this...

a __player_action__, when applied to a __game_state__, will produce a new __game_state__,
as well as a list of __game_state_changes__

__player_action__:
    - play a card from the hand
    - activate the leader card
    - pass turn

__game_state_changes__:
    - card moved to combat row from hand (player action)
    - card moved from deck to combat row (muster ability)
    - card moved from combat row to graveyard (scorch ability)
    - card moved from deck to hand (draw from spy ability)
    - round reset (lots stuff...)


*/

namespace ng
{
    struct Action;
    using actionlist_t = std::vector<Action>;

    struct GameChange;
    using gamechangelist_t = std::vector<GameChange>;

    struct LeaderAction { };
    struct PassTurnAction { };

    struct CardPlayAction
    {
        enum Row
        {
            ROW_MELEE, ROW_RANGED, ROW_SIEGE
        } row;

    };

    struct Action
    {
        enum Player
        {
            PLAYER_1, PLAYER_2
        } player;

        union
        {
            LeaderAction        u_leader;
            CardPlayAction      u_card;
            PassTurnAction      u_pass_turn;
        } value;
    };

    struct GameChange
    {

    };
}