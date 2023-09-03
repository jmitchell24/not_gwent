//
// Created by james on 9/1/23.
//

#include "ng/ng_card.hpp"
using namespace ng;

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

//
// Card -> Implementation
//

string Card::toString() const
{
    return name.c_str();
}

Card Card::makeNil()
{
    return Card{0, "nil"_sv, {}, EXPANSION_BASE_GAME, DECK_NEUTRAL, TYPE_NIL};
}

Card Card::makeUnit(unsigned id, cstrparam name, cstrparam filename, Expansion expansion, Deck deck, UnitCard unit)
{
    Card card{id, name, filename, expansion, deck, TYPE_UNIT};
    card.value.u_unit = unit;
    return card;
}

Card Card::makeLeader(unsigned id, cstrparam name, cstrparam filename, Expansion expansion, Deck deck, LeaderCard leader)
{
    Card card{id, name, filename, expansion, deck, TYPE_LEADER};
    card.value.u_leader = leader;
    return card;
}

Card Card::makeSpecial(unsigned id, cstrparam name, cstrparam filename, Expansion expansion, Deck deck, SpecialCard special)
{
    Card card{id, name, filename, expansion, DECK_NEUTRAL, TYPE_SPECIAL};
    card.value.u_special = special;
    return card;
}