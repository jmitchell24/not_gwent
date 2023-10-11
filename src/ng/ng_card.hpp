//
// Created by james on 9/1/23.
//

#pragma once

#include "check.hpp"

#include "ng/ng_enum.hpp"

//
// ut
//
#include <ut/string.hpp>

//
// std
//
#include <vector>

namespace ng
{
    struct Card;
    using cardlist_t = std::vector<Card>;

    struct NilCard { };

    struct LeaderCard
    {
        LeaderType type;
    };

    struct SpecialCard
    {
        SpecialType type;
    };

    struct UnitCard
    {
        unsigned        strength;
        bool            is_hero;

        AbilityType ability;
        RowFlag     row;
    };

    struct Card
    {
        unsigned        id;
        ut::cstrview    name;
        ut::cstrview    filename;

        CardType        type;
        DeckType        deck;
        ExpansionType   expansion;

        union
        {
#define CASE(a_, b_, c_, d_) c_ b_;
NG_ENUM_CARD_TYPE
#undef CASE
        } value;

#define CASE(a_, b_, c_, d_) \
        inline c_&       as##c_()       { assert(type == (CARD_##b_)); return value.b_; }  \
        inline c_ const& as##c_() const { assert(type == (CARD_##b_)); return value.b_; }  \
        inline bool      is##c_() const { return type == (CARD_##b_); }
NG_ENUM_CARD_TYPE
#undef CASE

/*
        void operator()(ng::NilCard const& c) { }
        void operator()(ng::SpecialCard const& c) { }
        void operator()(ng::UnitCard const& c) { }
        void operator()(ng::LeaderCard const& c) { }
 */
        template <typename T> inline auto visit(T&& t) const
        {
            switch (type)
            {
#define CASE(a_, b_, c_, d_) case CARD_##b_: return t(value.b_); break;
NG_ENUM_CARD_TYPE
#undef CASE
                default:nopath_case(CardType);
            }
        }

        static Card makeNil    ();
        static Card makeUnit   (unsigned id, ut::cstrparam name, ut::cstrparam filename, ExpansionType expansion, DeckType deck, UnitCard    unit   );
        static Card makeLeader (unsigned id, ut::cstrparam name, ut::cstrparam filename, ExpansionType expansion, DeckType deck, LeaderCard  leader );
        static Card makeSpecial(unsigned id, ut::cstrparam name, ut::cstrparam filename, ExpansionType expansion, DeckType deck, SpecialCard special);
    };
}



























