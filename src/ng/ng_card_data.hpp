#pragma once

// id, expansion, deck, symbol, name, texture name
#define NG_ENUM_CARDS_LEADERS_MONSTERS \
    CASE(1 , BASE_GAME      , MONSTERS       , MONSTERS_EREDIN_1             , monsters_eredin_1             , "Eredin: Bringer of Death"              , "monsters_eredin_1.png"            , EREDIN_1) \
    CASE(2 , BASE_GAME      , MONSTERS       , MONSTERS_EREDIN_2             , monsters_eredin_2             , "Eredin: Commander of the Red Riders"   , "monsters_eredin_2.png"            , EREDIN_2) \
    CASE(3 , BASE_GAME      , MONSTERS       , MONSTERS_EREDIN_3             , monsters_eredin_3             , "Eredin: Destroyer of Worlds"           , "monsters_eredin_3.png"            , EREDIN_3) \
    CASE(4 , BASE_GAME      , MONSTERS       , MONSTERS_EREDIN_4             , monsters_eredin_4             , "Eredin: King of the Wild Hunt"         , "monsters_eredin_4.png"            , EREDIN_4) \
    CASE(5 , HEARTS_OF_STONE, MONSTERS       , MONSTERS_EREDIN_5             , monsters_eredin_5             , "Eredin Bréacc Glas: The Treacherous"   , "monsters_eredin_5.png"            , EREDIN_5)

#define NG_ENUM_CARDS_SPECIALS \
    CASE(63, BASE_GAME      , NEUTRAL        , SPECIAL_FROST_1               , special_frost_1               , "Biting Frost (1 of 3)"                 , "special_frost.png"                , BITING_FROST) \
    CASE(64, BASE_GAME      , NEUTRAL        , SPECIAL_FROST_2               , special_frost_2               , "Biting Frost (2 of 3)"                 , "special_frost.png"                , BITING_FROST) \
    CASE(65, BASE_GAME      , NEUTRAL        , SPECIAL_FROST_3               , special_frost_3               , "Biting Frost (3 of 3)"                 , "special_frost.png"                , BITING_FROST) \
    CASE(66, BASE_GAME      , NEUTRAL        , SPECIAL_CLEARSKY_1            , special_clearsky_1            , "Clear Weather (1 of 3)"                , "special_clearsky.png"             , CLEAR_WEATHER) \
    CASE(67, BASE_GAME      , NEUTRAL        , SPECIAL_CLEARSKY_2            , special_clearsky_2            , "Clear Weather (2 of 3)"                , "special_clearsky.png"             , CLEAR_WEATHER) \
    CASE(68, BASE_GAME      , NEUTRAL        , SPECIAL_CLEARSKY_3            , special_clearsky_3            , "Clear Weather (3 of 3)"                , "special_clearsky.png"             , CLEAR_WEATHER) \
    CASE(69, BASE_GAME      , NEUTRAL        , SPECIAL_HORN_1                , special_horn_1                , "Commander's Horn (1 of 3)"             , "special_horn.png"                 , CMDR_HORN) \
    CASE(70, BASE_GAME      , NEUTRAL        , SPECIAL_HORN_2                , special_horn_2                , "Commander's Horn (2 of 3)"             , "special_horn.png"                 , CMDR_HORN) \
    CASE(71, BASE_GAME      , NEUTRAL        , SPECIAL_HORN_3                , special_horn_3                , "Commander's Horn (3 of 3)"             , "special_horn.png"                 , CMDR_HORN) \
    CASE(72, BASE_GAME      , NEUTRAL        , SPECIAL_DUMMY_1               , special_dummy_1               , "Decoy (1 of 3)"                        , "special_dummy.png"                , DECOY) \
    CASE(73, BASE_GAME      , NEUTRAL        , SPECIAL_DUMMY_2               , special_dummy_2               , "Decoy (2 of 3)"                        , "special_dummy.png"                , DECOY) \
    CASE(74, BASE_GAME      , NEUTRAL        , SPECIAL_DUMMY_3               , special_dummy_3               , "Decoy (3 of 3)"                        , "special_dummy.png"                , DECOY) \
    CASE(75, BASE_GAME      , NEUTRAL        , SPECIAL_FOG_1                 , special_fog_1                 , "Impenetrable Fog (1 of 3)"             , "special_fog.png"                  , IMP_FOG) \
    CASE(76, BASE_GAME      , NEUTRAL        , SPECIAL_FOG_2                 , special_fog_2                 , "Impenetrable Fog (2 of 3)"             , "special_fog.png"                  , IMP_FOG) \
    CASE(77, BASE_GAME      , NEUTRAL        , SPECIAL_FOG_3                 , special_fog_3                 , "Impenetrable Fog (3 of 3)"             , "special_fog.png"                  , IMP_FOG) \
    CASE(78, BASE_GAME      , NEUTRAL        , SPECIAL_SCORCH_1              , special_scorch_1              , "Scorch (1 of 3)"                       , "special_scorch.png"               , SCORCH) \
    CASE(79, BASE_GAME      , NEUTRAL        , SPECIAL_SCORCH_2              , special_scorch_2              , "Scorch (2 of 3)"                       , "special_scorch.png"               , SCORCH) \
    CASE(80, BASE_GAME      , NEUTRAL        , SPECIAL_SCORCH_3              , special_scorch_3              , "Scorch (3 of 3)"                       , "special_scorch.png"               , SCORCH) \
    CASE(81, BASE_GAME      , NEUTRAL        , SPECIAL_RAIN_1                , special_rain_1                , "Torrential Rain (1 of 3)"              , "special_rain.png"                 , TOR_RAIN) \
    CASE(82, BASE_GAME      , NEUTRAL        , SPECIAL_RAIN_2                , special_rain_2                , "Torrential Rain (2 of 3)"              , "special_rain.png"                 , TOR_RAIN) \
    CASE(83, BASE_GAME      , NEUTRAL        , SPECIAL_RAIN_3                , special_rain_3                , "Torrential Rain (3 of 3)"              , "special_rain.png"                 , TOR_RAIN) \
    CASE(84, BLOOD_AND_WINE , NEUTRAL        , SPECIAL_SKELLIGE_STORM_1      , special_skellige_storm_1      , "Skellige Storm (1 of 3)"               , "special_skellige_storm.png"       , SKELLIGE_STORM) \
    CASE(85, BLOOD_AND_WINE , NEUTRAL        , SPECIAL_SKELLIGE_STORM_2      , special_skellige_storm_2      , "Skellige Storm (2 of 3)"               , "special_skellige_storm.png"       , SKELLIGE_STORM) \
    CASE(86, BLOOD_AND_WINE , NEUTRAL        , SPECIAL_SKELLIGE_STORM_3      , special_skellige_storm_3      , "Skellige Storm (3 of 3)"               , "special_skellige_storm.png"       , SKELLIGE_STORM)


// id, expansion, deck, symbol, name, strength, is hero?, combat row, ability, texture name
#define NG_ENUM_CARDS_UNITS_MONSTERS \
    CASE(6 , BASE_GAME      , MONSTERS       , MONSTERS_DRAUG                , monsters_draug                , "Draug"                                 , "monsters_draug.png"               , 10, true  , MELEE          , NONE    ) \
    CASE(7 , BASE_GAME      , MONSTERS       , MONSTERS_IMLERITH             , monsters_imlerith             , "Imlerith"                              , "monsters_imlerith.png"            , 10, true  , MELEE          , NONE    ) \
    CASE(8 , BASE_GAME      , MONSTERS       , MONSTERS_KAYRAN               , monsters_kayran               , "Kayran"                                , "monsters_kayran.png"              , 18, true  , MELEE_RANGED   , MORALE  ) \
    CASE(9 , BASE_GAME      , MONSTERS       , MONSTERS_LESHAN               , monsters_leshan               , "Leshen"                                , "monsters_leshan.png"              , 10, true  , RANGED         , NONE    ) \
    CASE(10, BASE_GAME      , MONSTERS       , MONSTERS_ARACHAS_1            , monsters_arachas_1            , "Arachas (1 of 3)"                      , "monsters_arachas_1.png"           , 4 , false , MELEE          , MUSTER  ) \
    CASE(11, BASE_GAME      , MONSTERS       , MONSTERS_ARACHAS_2            , monsters_arachas_2            , "Arachas (2 of 3)"                      , "monsters_arachas_2.png"           , 4 , false , MELEE          , MUSTER  ) \
    CASE(12, BASE_GAME      , MONSTERS       , MONSTERS_ARACHAS_3            , monsters_arachas_3            , "Arachas (3 of 3)"                      , "monsters_arachas_3.png"           , 4 , false , MELEE          , MUSTER  ) \
    CASE(13, BASE_GAME      , MONSTERS       , MONSTERS_ARACHAS_BEHEMOTH     , monsters_arachas_behemoth     , "Arachas Behemoth"                      , "monsters_arachas_behemoth.png"    , 6 , false , SIEGE          , MUSTER  ) \
    CASE(14, BASE_GAME      , MONSTERS       , MONSTERS_BOTCHLING            , monsters_botchling            , "Botchling"                             , "monsters_botchling.png"           , 4 , false , MELEE          , NONE    ) \
    CASE(15, BASE_GAME      , MONSTERS       , MONSTERS_CELAENO_HARPY        , monsters_celaeno_harpy        , "Celaeno Harpy"                         , "monsters_celaeno_harpy.png"       , 2 , false , MELEE_RANGED   , AGILE   ) \
    CASE(16, BASE_GAME      , MONSTERS       , MONSTERS_COCKATRICE           , monsters_cockatrice           , "Cockatrice"                            , "monsters_cockatrice.png"          , 2 , false , RANGED         , NONE    ) \
    CASE(17, BASE_GAME      , MONSTERS       , MONSTERS_CRONE_1              , monsters_crone_1              , "Crone: Brewess"                        , "monsters_crone_1.png"             , 6 , false , MELEE          , MUSTER  ) \
    CASE(18, BASE_GAME      , MONSTERS       , MONSTERS_CRONE_2              , monsters_crone_2              , "Crone: Weavess"                        , "monsters_crone_2.png"             , 6 , false , MELEE          , MUSTER  ) \
    CASE(19, BASE_GAME      , MONSTERS       , MONSTERS_CRONE_3              , monsters_crone_3              , "Crone: Whispess"                       , "monsters_crone_3.png"             , 6 , false , MELEE          , MUSTER  ) \
    CASE(20, BASE_GAME      , MONSTERS       , MONSTERS_EARTH_ELEMENTAL      , monsters_earth_elemental      , "Earth Elemental"                       , "monsters_earth_elemental.png"     , 6 , false , SIEGE          , NONE    ) \
    CASE(21, BASE_GAME      , MONSTERS       , MONSTERS_ENDREGA              , monsters_endrega              , "Endrega"                               , "monsters_endrega.png"             , 2 , false , RANGED         , NONE    ) \
    CASE(22, BASE_GAME      , MONSTERS       , MONSTERS_FIEND                , monsters_fiend                , "Fiend"                                 , "monsters_fiend.png"               , 6 , false , MELEE          , NONE    ) \
    CASE(23, BASE_GAME      , MONSTERS       , MONSTERS_FIRE_ELEMENTAL       , monsters_fire_elemental       , "Fire Elemental"                        , "monsters_fire_elemental.png"      , 6 , false , SIEGE          , NONE    ) \
    CASE(24, BASE_GAME      , MONSTERS       , MONSTERS_FOGLET               , monsters_foglet               , "Foglet"                                , "monsters_foglet.png"              , 2 , false , MELEE          , NONE    ) \
    CASE(25, BASE_GAME      , MONSTERS       , MONSTERS_FORKTAIL             , monsters_forktail             , "Forktail"                              , "monsters_forktail.png"            , 5 , false , MELEE          , NONE    ) \
    CASE(26, BASE_GAME      , MONSTERS       , MONSTERS_FRIGHTENER           , monsters_frightener           , "Frightener"                            , "monsters_frightener.png"          , 5 , false , MELEE          , NONE    ) \
    CASE(27, BASE_GAME      , MONSTERS       , MONSTERS_GARGOYLE             , monsters_gargoyle             , "Gargoyle"                              , "monsters_gargoyle.png"            , 2 , false , RANGED         , NONE    ) \
    CASE(28, BASE_GAME      , MONSTERS       , MONSTERS_GHOUL_1              , monsters_ghoul_1              , "Ghoul (1 of 3)"                        , "monsters_ghoul_1.png"             , 1 , false , MELEE          , MUSTER  ) \
    CASE(29, BASE_GAME      , MONSTERS       , MONSTERS_GHOUL_2              , monsters_ghoul_2              , "Ghoul (2 of 3)"                        , "monsters_ghoul_2.png"             , 1 , false , MELEE          , MUSTER  ) \
    CASE(30, BASE_GAME      , MONSTERS       , MONSTERS_GHOUL_3              , monsters_ghoul_3              , "Ghoul (3 of 3)"                        , "monsters_ghoul_3.png"             , 1 , false , MELEE          , MUSTER  ) \
    CASE(31, BASE_GAME      , MONSTERS       , MONSTERS_GRAVEHAG             , monsters_gravehag             , "Grave Hag"                             , "monsters_gravehag.png"            , 5 , false , RANGED         , NONE    ) \
    CASE(32, BASE_GAME      , MONSTERS       , MONSTERS_GRYFFIN              , monsters_gryffin              , "Griffin"                               , "monsters_gryffin.png"             , 5 , false , MELEE          , NONE    ) \
    CASE(33, BASE_GAME      , MONSTERS       , MONSTERS_HARPY                , monsters_harpy                , "Harpy"                                 , "monsters_harpy.png"               , 2 , false , MELEE_RANGED   , AGILE   ) \
    CASE(34, BASE_GAME      , MONSTERS       , MONSTERS_FROST_GIANT          , monsters_frost_giant          , "Ice Giant"                             , "monsters_frost_giant.png"         , 5 , false , SIEGE          , NONE    ) \
    CASE(34, HEARTS_OF_STONE, MONSTERS       , MONSTERS_TOAD                 , monsters_toad                 , "Toad"                                  , "monsters_toad.png"                , 7 , false , RANGED         , SCORCH  ) \
    CASE(36, BASE_GAME      , MONSTERS       , MONSTERS_NEKKER_1             , monsters_nekker_1             , "Nekker (1 of 3)"                       , "monsters_nekker_1.png"            , 2 , false , MELEE          , MUSTER  ) \
    CASE(37, BASE_GAME      , MONSTERS       , MONSTERS_NEKKER_2             , monsters_nekker_2             , "Nekker (2 of 3)"                       , "monsters_nekker_2.png"            , 2 , false , MELEE          , MUSTER  ) \
    CASE(38, BASE_GAME      , MONSTERS       , MONSTERS_NEKKER_3             , monsters_nekker_3             , "Nekker (3 of 3)"                       , "monsters_nekker_3.png"            , 2 , false , MELEE          , MUSTER  ) \
    CASE(39, BASE_GAME      , MONSTERS       , MONSTERS_PLAGUE_MAIDEN        , monsters_plague_maiden        , "Plague Maiden"                         , "monsters_plague_maiden.png"       , 5 , false , MELEE          , NONE    ) \
    CASE(40, BASE_GAME      , MONSTERS       , MONSTERS_VAMP_BRUXA           , monsters_vamp_bruxa           , "Vampire: Bruxa"                        , "monsters_vamp_bruxa.png"          , 4 , false , MELEE          , MUSTER  ) \
    CASE(41, BASE_GAME      , MONSTERS       , MONSTERS_VAMP_EKIMMARA        , monsters_vamp_ekimmara        , "Vampire: Ekimmara"                     , "monsters_vamp_ekimmara.png"       , 4 , false , MELEE          , MUSTER  ) \
    CASE(42, BASE_GAME      , MONSTERS       , MONSTERS_VAMP_FLEDER          , monsters_vamp_fleder          , "Vampire: Fleder"                       , "monsters_vamp_fleder.png"         , 4 , false , MELEE          , MUSTER  ) \
    CASE(43, BASE_GAME      , MONSTERS       , MONSTERS_VAMP_GARKAIN         , monsters_vamp_garkain         , "Vampire: Garkain"                      , "monsters_vamp_garkain.png"        , 4 , false , MELEE          , MUSTER  ) \
    CASE(44, BASE_GAME      , MONSTERS       , MONSTERS_VAMP_KATAKAN         , monsters_vamp_katakan         , "Vampire: Katakan"                      , "monsters_vamp_katakan.png"        , 5 , false , MELEE          , MUSTER  ) \
    CASE(45, BASE_GAME      , MONSTERS       , MONSTERS_WEREWOLF             , monsters_werewolf             , "Werewolf"                              , "monsters_werewolf.png"            , 5 , false , MELEE          , NONE    ) \
    CASE(46, BASE_GAME      , MONSTERS       , MONSTERS_WYVERN               , monsters_wyvern               , "Wyvern"                                , "monsters_wyvern.png"              , 2 , false , RANGED         , NONE    )

#define NG_ENUM_CARDS_UNITS_NEUTRAL \
    CASE(47, BASE_GAME      , NEUTRAL        , NEUTRAL_CIRI_1                , neutral_ciri_1                , "Cirilla Fiona Elen Riannon"            , "neutral_ciri_1.png"               , 15, true  , MELEE          , NONE    ) \
    CASE(48, BASE_GAME      , NEUTRAL        , NEUTRAL_DANDELION             , neutral_dandelion             , "Dandelion"                             , "neutral_dandelion.png"            , 2 , true  , MELEE          , MORALE  ) \
    CASE(49, BASE_GAME      , NEUTRAL        , NEUTRAL_GERALT                , neutral_geralt                , "Geralt of Rivia"                       , "neutral_geralt.png"               , 15, true  , MELEE          , NONE    ) \
    CASE(50, BASE_GAME      , NEUTRAL        , NEUTRAL_AVALLACH              , neutral_avallach              , "Mysterious Elf"                        , "neutral_avallach.png"             , 0 , true  , MELEE          , SPY     ) \
    CASE(51, BASE_GAME      , NEUTRAL        , NEUTRAL_TRISS                 , neutral_triss                 , "Triss Merigold"                        , "neutral_triss.png"                , 7 , true  , MELEE          , NONE    ) \
    CASE(52, BASE_GAME      , NEUTRAL        , NEUTRAL_YENNEFER              , neutral_yennefer              , "Yennefer of Vengerberg"                , "neutral_yennefer.png"             , 7 , true  , RANGED         , MEDIC   ) \
    CASE(53, BASE_GAME      , NEUTRAL        , NEUTRAL_EMIEL                 , neutral_emiel                 , "Emiel Regis Rohellec Terzieff"         , "neutral_emiel.png"                , 5 , false , MELEE          , NONE    ) \
    CASE(54, BASE_GAME      , NEUTRAL        , NEUTRAL_VESEMIR               , neutral_vesemir               , "Vesemir"                               , "neutral_vesemir.png"              , 6 , false , MELEE          , NONE    ) \
    CASE(55, BASE_GAME      , NEUTRAL        , NEUTRAL_VILLEN                , neutral_villen                , "Villentretenmerth"                     , "neutral_villen.png"               , 7 , false , MELEE          , SCORCH  ) \
    CASE(56, BASE_GAME      , NEUTRAL        , NEUTRAL_ZOLTAN                , neutral_zoltan                , "Zoltan Chivay"                         , "neutral_zoltan.png"               , 5 , false , MELEE          , NONE    ) \
    CASE(57, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_COW                   , neutral_cow                   , "Cow"                                   , "neutral_cow.png"                  , 0 , false , RANGED         , NONE    ) \
    CASE(58, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_GAUNTER_DARKNESS_1    , neutral_gaunter_darkness_1    , "Gaunter O'Dimm: Darkness (1 of 3)"     , "neutral_gaunter_darkness.png"     , 4 , false , RANGED         , MUSTER  ) \
    CASE(59, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_GAUNTER_DARKNESS_2    , neutral_gaunter_darkness_2    , "Gaunter O'Dimm: Darkness (2 of 3)"     , "neutral_gaunter_darkness.png"     , 4 , false , RANGED         , MUSTER  ) \
    CASE(60, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_GAUNTER_DARKNESS_3    , neutral_gaunter_darkness_3    , "Gaunter O'Dimm: Darkness (3 of 3)"     , "neutral_gaunter_darkness.png"     , 4 , false , RANGED         , MUSTER  ) \
    CASE(61, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_GAUNTER               , neutral_gaunter               , "Gaunter O'Dimm"                        , "neutral_gaunter.png"              , 2 , false , SIEGE          , MUSTER  ) \
    CASE(62, HEARTS_OF_STONE, NEUTRAL        , NEUTRAL_OLGIERD               , neutral_olgierd               , "Olgierd von Everec"                    , "neutral_olgierd.png"              , 6 , false , MELEE_RANGED   , MORALE  )


#define NG_ENUM_CARDS_UNITS         \
    NG_ENUM_CARDS_UNITS_MONSTERS    \
    NG_ENUM_CARDS_UNITS_NEUTRAL

#define NG_ENUM_CARDS       \
    NG_ENUM_CARDS_UNITS     \
    NG_ENUM_CARDS_SPECIALS

#include "ng/ng_card.hpp"

namespace ng
{
namespace cards
{
#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9, _p10) \
    static const Card _p4 = Card::makeUnit(_p0, _p5, _p6, Card::EXPANSION_##_p1, Card::DECK_##_p2, { _p7, _p8, UnitCard::ABILITY_##_p10, UnitCard::ROW_##_p9 });
NG_ENUM_CARDS_UNITS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7) \
    static const Card _p4 = Card::makeSpecial(_p0, _p5, _p6, Card::EXPANSION_##_p1, Card::DECK_##_p2, { SpecialCard::SPECIAL_##_p7 });
NG_ENUM_CARDS_SPECIALS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7) \
    static const Card _p4 = Card::makeLeader(_p0, _p5, _p6, Card::EXPANSION_##_p1, Card::DECK_##_p2, { LeaderCard::LEADER_##_p7 });
NG_ENUM_CARDS_LEADERS_MONSTERS
#undef CASE
}

    enum CardID
    {
#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7)  CARD_##_p3,
NG_ENUM_CARDS_LEADERS_MONSTERS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7)  CARD_##_p3,
NG_ENUM_CARDS_SPECIALS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9, _p10) CARD_##_p3,
NG_ENUM_CARDS_UNITS
#undef CASE

        CARD_COUNT_
    };

    inline Card getCard(CardID id)
    {
        switch (id)
        {
#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7) case CARD_##_p3: return cards::_p4;
NG_ENUM_CARDS_LEADERS_MONSTERS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7) case CARD_##_p3: return cards::_p4;
NG_ENUM_CARDS_SPECIALS
#undef CASE

#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9, _p10) case CARD_##_p3: return cards::_p4;
NG_ENUM_CARDS_UNITS
#undef CASE

            default:assert_case(CardID);
        }

        return Card::makeNil();
    }
}
