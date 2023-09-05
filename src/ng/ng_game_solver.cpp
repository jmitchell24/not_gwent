//
// Created by james on 9/1/23.
//

#include "ng/ng_game_solver.hpp"
using namespace ng;

//
// std
//
using namespace std;

bool GameSolver::applyAction(Action const& action, gamechangelist_t& changes)
{

    return false;
}

static cardlist_t makeDeck()
{
    cardlist_t cards
    {
#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9, _p10) cards::_p4,
NG_ENUM_CARDS_UNITS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7) cards::_p4,
NG_ENUM_CARDS_SPECIALS
#undef CASE
    };

    random_shuffle(cards.begin(), cards.end(), [](int x){ return rand()%x;  });

    return cards;
}

static Player makePlayer(Card const& leader)
{
    assert(leader.isLeaderCard());

    Player player;
    player.leader = leader;
    player.deck = makeDeck();
    //TODO: move 10 cards from end of deck into hand
    player.hand.resize(10);
    copy(player.deck.rbegin(), player.deck.rbegin()+10, player.hand.begin());
    player.deck.resize(player.deck.size()-10);


    return player;
}

Game GameSolver::makeGame()
{
    return {makePlayer(cards::monsters_eredin_1), makePlayer(cards::monsters_eredin_2)};
}