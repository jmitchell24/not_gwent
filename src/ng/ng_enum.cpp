//
// Created by james on 9/30/23.
//

#include "ng/ng_enum.hpp"
using namespace ng;

#include "check.hpp"

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;


cstrview ng::toString(CardType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_CARD_TYPE
#undef CASE
        default: nopath_case(CardType);
    }
    return ""_sv;
}

cstrview ng::toString(LeaderType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_LEADER_TYPE
#undef CASE
        default: nopath_case(LeaderType);
    }
    return ""_sv;
}

cstrview ng::toString(SpecialType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_SPECIAL_TYPE
#undef CASE
        default: nopath_case(SpecialType);
    }
    return ""_sv;
}

cstrview ng::toString(AbilityType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_ABILITY_TYPE
#undef CASE
        default: nopath_case(AbilityType);
    }
    return ""_sv;
}

cstrview ng::toString(DeckType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_DECK_TYPE
#undef CASE
        default: nopath_case(DeckType);
    }
    return ""_sv;
}

cstrview ng::toString(ExpansionType type)
{
    switch (type)
    {
#define CASE(a_, b_, c_, d_) case a_: return d_##_sv;
NG_ENUM_EXPANSION_TYPE
#undef CASE
        default: nopath_case(ExpansionType);
    }
    return ""_sv;
}

string ng::toString(RowFlag flag)
{
    string s,d;

#define CASE(a_, b_, c_, d_) if (flag & a_) { s += d + d_; d = "|"; }
NG_FLAG_ROW
#undef CASE
    return s;
}