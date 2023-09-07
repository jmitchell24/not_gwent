//
// Created by james on 9/1/23.
//

#pragma once

#include "assert.hpp"

//
// ut
//
#include <ut/string.hpp>

//
// std
//
#include <vector>

#define NG_ENUM_CARD_TYPE \
    CASE(NilCard        , NIL       , u_nil     , "---"         )  \
    CASE(SpecialCard    , SPECIAL   , u_special , "Special"     )  \
    CASE(UnitCard       , UNIT      , u_unit    , "Unit"        )  \
    CASE(LeaderCard     , LEADER    , u_leader  , "Leader"      )

#define NG_ENUM_LEADER_TYPE \
    CASE(EREDIN_1, Eredin1 , "Eredin: 1"           ) \
    CASE(EREDIN_2, Eredin2 , "Eredin: 2"           ) \
    CASE(EREDIN_3, Eredin3 , "Eredin: 3"           ) \
    CASE(EREDIN_4, Eredin4 , "Eredin: 4"           ) \
    CASE(EREDIN_5, Eredin5 , "Eredin: 5"           )

#define NG_ENUM_SPECIAL_TYPE \
    CASE(DECOY          , Decoy         , "Decoy"           ) \
    CASE(CMDR_HORN      , CmdrHorn      , "Commander Horn"  ) \
    CASE(SCORCH         , Scorch        , "Scorch"          ) \
    CASE(BITING_FROST   , BitingFrost   , "Biting Frost"    ) \
    CASE(IMP_FOG        , ImpFog        , "Impenetrable Fog") \
    CASE(TOR_RAIN       , TorRain       , "Torrential Rain" ) \
    CASE(CLEAR_WEATHER  , ClearWeather  , "ClearWeather"    ) \
    CASE(SKELLIGE_STORM , SkelligeStorm , "Skellige Storm"  )

#define NG_ENUM_ABILITY \
    CASE(NONE       , None      , "---"         ) \
    CASE(SPY        , Spy       , "Spy"         ) \
    CASE(BOND       , Bond      , "Bond"        ) \
    CASE(MORALE     , Morale    , "Morale"      ) \
    CASE(MEDIC      , Medic     , "Medic"       ) \
    CASE(AGILE      , Agile     , "Agile"       ) \
    CASE(MUSTER     , Muster    , "Muster"      ) \
    CASE(SCORCH     , Scorch    , "Scorch"      ) \
    CASE(BERSERKER  , Berserker , "Berserker"   )

#define NG_ENUM_ROW \
    CASE(MELEE          , Melee         , "Melee"           ) \
    CASE(RANGED         , Ranged        , "Ranged"          ) \
    CASE(MELEE_RANGED   , MeleeRanged   , "Melee & Ranged"  ) \
    CASE(SIEGE          , Siege         , "Siege"           )

#define NG_ENUM_DECK \
    CASE(NEUTRAL        , Neutral       , "Neutral"         ) \
    CASE(NORTHERN_REALMS, NorthernRealms, "Northern Realms" ) \
    CASE(NILFGAARD      , Nilfgaard     , "Nilfgaard"       ) \
    CASE(SCOIATAEL      , Scoiatael     , "Scoiatael"       ) \
    CASE(MONSTERS       , Monsters      , "Monsters"        ) \
    CASE(SKELLIGE       , Skellige      , "Skellige"        )

#define NG_ENUM_EXPANSION \
    CASE(BASE_GAME      , BaseGame      , "Base Game"       ) \
    CASE(HEARTS_OF_STONE, HeartOfStone  , "Heart of Stone"  ) \
    CASE(BLOOD_AND_WINE , BloodAndWine  , "Blood and Wine"  )

namespace ng
{
    struct Card;
    using cardlist_t = std::vector<Card>;

    struct NilCard { };

    struct LeaderCard
    {
        enum Type
        {
#define CASE(x_, y_, z_) LEADER_##x_,
NG_ENUM_LEADER_TYPE
#undef CASE
        } type;
    };

    struct SpecialCard
    {
        enum Type
        {
#define CASE(x_, y_, z_) SPECIAL_##x_,
NG_ENUM_SPECIAL_TYPE
#undef CASE
        } type;
    };

    struct UnitCard
    {
        unsigned        strength;
        bool            is_hero;

        enum Ability
        {
#define CASE(x_, y_, z_) ABILITY_##x_,
NG_ENUM_ABILITY
#undef CASE

        } ability;

        enum Row
        {
#define CASE(x_, y_, z_) ROW_##x_,
NG_ENUM_ROW
#undef CASE
        } row;
    };

    // TODO: add a 'toString' or equivalent for console printout
    struct Card
    {
        unsigned        id;
        ut::cstrview    name;
        ut::cstrview    filename;

        enum Expansion
        {
#define CASE(x_, y_, z_) EXPANSION_##x_,
NG_ENUM_EXPANSION
#undef CASE
        } expansion;

        enum Deck
        {
#define CASE(x_, y_, z_) DECK_##x_,
NG_ENUM_DECK
#undef CASE
        } deck;

        enum Type
        {
#define CASE(x_, y_, z_, w_) TYPE_##y_,
NG_ENUM_CARD_TYPE
#undef CASE

        } type;

        union
        {
#define CASE(x_, y_, z_, w_) x_ z_;
NG_ENUM_CARD_TYPE
#undef CASE
        } value;

#define CASE(x_, y_, z_, w_)                                                        \
        inline x_&       as##x_()       { assert(type == (TYPE_##y_)); return value.z_; }  \
        inline x_ const& as##x_() const { assert(type == (TYPE_##y_)); return value.z_; }  \
        inline bool      is##x_() const { return type == (TYPE_##y_); }
NG_ENUM_CARD_TYPE
#undef CASE

        std::string toString() const;

        static Card makeNil    ();
        static Card makeUnit   (unsigned id, ut::cstrparam name, ut::cstrparam filename, Expansion expansion, Deck deck, UnitCard    unit   );
        static Card makeLeader (unsigned id, ut::cstrparam name, ut::cstrparam filename, Expansion expansion, Deck deck, LeaderCard  leader );
        static Card makeSpecial(unsigned id, ut::cstrparam name, ut::cstrparam filename, Expansion expansion, Deck deck, SpecialCard special);
    };
}