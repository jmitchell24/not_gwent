//
// Created by james on 9/30/23.
//

#pragma once

#include <ut/string.hpp>

#define NG_ENUM_CARD_TYPE \
    CASE(0, NIL    , NilCard    , "---"         )  \
    CASE(1, SPECIAL, SpecialCard, "Special"     )  \
    CASE(2, UNIT   , UnitCard   , "Unit"        )  \
    CASE(3, LEADER , LeaderCard , "Leader"      )

#define NG_ENUM_LEADER_TYPE \
    CASE(0, EREDIN_1, Eredin1 , "Eredin: 1"           ) \
    CASE(1, EREDIN_2, Eredin2 , "Eredin: 2"           ) \
    CASE(2, EREDIN_3, Eredin3 , "Eredin: 3"           ) \
    CASE(3, EREDIN_4, Eredin4 , "Eredin: 4"           ) \
    CASE(4, EREDIN_5, Eredin5 , "Eredin: 5"           )

#define NG_ENUM_SPECIAL_TYPE \
    CASE(0, DECOY          , Decoy        , "Decoy"           ) \
    CASE(1, CMDR_HORN      , CmdrHorn     , "Commander Horn"  ) \
    CASE(2, SCORCH         , Scorch       , "Scorch"          ) \
    CASE(3, BITING_FROST   , BitingFrost  , "Biting Frost"    ) \
    CASE(4, IMP_FOG        , ImpFog       , "Impenetrable Fog") \
    CASE(5, TOR_RAIN       , TorRain      , "Torrential Rain" ) \
    CASE(6, CLEAR_WEATHER  , ClearWeather , "ClearWeather"    ) \
    CASE(7, SKELLIGE_STORM , SkelligeStorm, "Skellige Storm"  )

#define NG_ENUM_ABILITY_TYPE \
    CASE(0, NONE     , None     , "---"         ) \
    CASE(1, SPY      , Spy      , "Spy"         ) \
    CASE(2, BOND     , Bond     , "Bond"        ) \
    CASE(3, MORALE   , Morale   , "Morale"      ) \
    CASE(4, MEDIC    , Medic    , "Medic"       ) \
    CASE(5, AGILE    , Agile    , "Agile"       ) \
    CASE(6, MUSTER   , Muster   , "Muster"      ) \
    CASE(7, SCORCH   , Scorch   , "Scorch"      ) \
    CASE(8, BERSERKER, Berserker, "Berserker"   )

#define NG_ENUM_DECK_TYPE \
    CASE(0, NEUTRAL        , Neutral       , "Neutral"         ) \
    CASE(1, NORTHERN_REALMS, NorthernRealms, "Northern Realms" ) \
    CASE(2, NILFGAARD      , Nilfgaard     , "Nilfgaard"       ) \
    CASE(3, SCOIATAEL      , Scoiatael     , "Scoiatael"       ) \
    CASE(4, MONSTERS       , Monsters      , "Monsters"        ) \
    CASE(5, SKELLIGE       , Skellige      , "Skellige"        )

#define NG_ENUM_EXPANSION_TYPE \
    CASE(0, BASE_GAME      , BaseGame      , "Base Game"       ) \
    CASE(1, HEARTS_OF_STONE, HeartOfStone  , "Heart of Stone"  ) \
    CASE(2, BLOOD_AND_WINE , BloodAndWine  , "Blood and Wine"  )

#define NG_FLAG_ROW \
    CASE(0b00000000, USR   , Usr   , "User"   ) \
    CASE(0b00000001, CPU   , Cpu   , "Cpu"    ) \
    CASE(0b00000010, MELEE , Melee , "Melee"  ) \
    CASE(0b00000100, RANGED, Ranged, "Ranged" ) \
    CASE(0b00001000, SIEGE , Siege , "Siege"  )



namespace ng
{
    enum CardType
    {
#define CASE(a_, b_, c_, d_) CARD_##b_ = a_,
NG_ENUM_CARD_TYPE
#undef CASE
    };

    enum LeaderType
    {
#define CASE(a_, b_, c_, d_) LEADER_##b_ = a_,
NG_ENUM_LEADER_TYPE
#undef CASE
    };

    enum SpecialType
    {
#define CASE(a_, b_, c_, d_) SPECIAL_##b_ = a_,
NG_ENUM_SPECIAL_TYPE
#undef CASE
    };

    enum AbilityType
    {
#define CASE(a_, b_, c_, d_) ABILITY_##b_ = a_,
NG_ENUM_ABILITY_TYPE
#undef CASE
    };

    enum DeckType
    {
#define CASE(a_, b_, c_, d_) DECK_##b_ = a_,
NG_ENUM_DECK_TYPE
#undef CASE
    };

    enum ExpansionType
    {
#define CASE(a_, b_, c_, d_) EXPANSION_##b_ = a_,
NG_ENUM_EXPANSION_TYPE
#undef CASE
    };

    enum RowFlag_
    {
#define CASE(a_, b_, c_, d_) ROW_##b_ = a_,
NG_FLAG_ROW
#undef CASE

        ROW_MELEE_RANGED = ROW_MELEE | ROW_RANGED
    };

    using RowFlag = int;

    ut::cstrview toString(CardType      type);
    ut::cstrview toString(LeaderType    type);
    ut::cstrview toString(SpecialType   type);
    ut::cstrview toString(AbilityType   type);
    ut::cstrview toString(DeckType      type);
    ut::cstrview toString(ExpansionType type);

    std::string toString(RowFlag flag);
}