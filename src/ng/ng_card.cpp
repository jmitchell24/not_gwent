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

Card Card::makeNil()
{
    return Card{0, "nil"_sv, {}, CARD_NIL, DECK_NEUTRAL, EXPANSION_BASE_GAME };
}

Card Card::makeUnit(unsigned id, cstrparam name, cstrparam filename, ExpansionType expansion, DeckType deck, UnitCard unit)
{
    Card card{ .id=id, .name=name, .filename=filename, .type=CARD_UNIT, .deck=deck, .expansion=expansion };
    card.value.UNIT = unit;
    return card;
}

Card Card::makeLeader(unsigned id, cstrparam name, cstrparam filename, ExpansionType expansion, DeckType deck, LeaderCard leader)
{
    Card card{ .id=id, .name=name, .filename=filename, .type=CARD_LEADER, .deck=deck, .expansion=expansion };
    card.value.LEADER = leader;
    return card;
}

Card Card::makeSpecial(unsigned id, cstrparam name, cstrparam filename, ExpansionType expansion, DeckType deck, SpecialCard special)
{
    Card card{ .id=id, .name=name, .filename=filename, .type=CARD_SPECIAL, .deck=deck, .expansion=expansion };
    card.value.SPECIAL = special;
    return card;
}






