#pragma once

#include <vector>
#include <cassert>

#define NG_ENUM_CARD_TYPE \
    CASE(NilCard        , NIL       , u_nil     , "---"         )  \
    CASE(UtilityCard    , UTILITY   , u_utility , "Utility"     )  \
    CASE(UnitCard       , UNIT      , u_unit    , "Unit"        )  \
    CASE(LeaderCard     , LEADER    , u_leader  , "Leader"      )

#define NG_ENUM_UTILITY_TYPE \
    CASE(DECOY          , Decoy         , "Decoy"           ) \
    CASE(CMDR_HORN      , CmdrHorn      , "Commander Horn"  ) \
    CASE(SCORCH         , Scorch        , "Scorch"          ) \
    CASE(BITING_FROST   , BitingFrost   , "Biting Frost"    ) \
    CASE(IMP_FOG        , ImpFog        , "Impenetrable Fog") \
    CASE(TOR_RAIN       , TorRain       , "Torrential Rain" ) \
    CASE(CLEAR_WEATHER  , ClearWeather  , "ClearWeather"    )

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
    CASE(NORTHERN_REALMS, NorthernRealms, "Northern Realms" ) \
    CASE(NILFGAARD      , Nilfgaard     , "Nilfgaard"       ) \
    CASE(SCOIATAEL      , Scoiatael     , "Scoiatael"       ) \
    CASE(MONSTERS       , Monsters      , "Monsters"        ) \
    CASE(SKELLIGE       , Skellige      , "Skellige"        )

#define NG_ENUM_EXPANSION \
    CASE(BASE_GAME      , BaseGame      , "Base Game"       ) \
    CASE(HEART_OF_STONE , HeartOfStone  , "Heart of Stone"  ) \
    CASE(BLOOD_AND_WINE , BloodAndWine  , "Blood and Wine"  )


namespace ng
{
    struct Card;
    using cardlist_t = std::vector<Card>;

    struct NilCard { };

    struct UtilityCard
    {
        enum Type
        {
#define CASE(x_, y_, z_) x_,
NG_ENUM_UTILITY_TYPE
#undef CASE
        } type;
    };

    struct UnitCard
    {
        unsigned    strength    = 0;
        bool        hero        = false;

        enum Ability
        {
#define CASE(x_, y_, z_) x_,
NG_ENUM_ABILITY
#undef CASE

        } ability;

        enum Row
        {
#define CASE(x_, y_, z_) x_,
NG_ENUM_ROW
#undef CASE
        } row;
    };

    struct LeaderCard
    {

    };

    struct Card
    {
        enum Expansion
        {
#define CASE(x_, y_, z_) x_,
NG_ENUM_EXPANSION
#undef CASE
        } expansion=BASE_GAME;

        enum Type
        {
#define CASE(x_, y_, z_, w_) y_,
NG_ENUM_CARD_TYPE
#undef CASE

        } type=NIL;

        unsigned id = 0;

        union
        {
#define CASE(x_, y_, z_, w_) x_ z_;
NG_ENUM_CARD_TYPE
#undef CASE
        } value;

#define CASE(x_, y_, z_, w_)                                                        \
        inline x_&       as##z_()       { assert(type == (y_)); return value.z_; }  \
        inline x_        as##z_() const { assert(type == (y_)); return value.z_; }  \
        inline bool      is##z_() const { return type == (y_); }
NG_ENUM_CARD_TYPE
#undef CASE
    };


}
