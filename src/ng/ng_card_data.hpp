#pragma once

// id, expansion, deck, symbol, name, texture name
#define NG_ENUM_CARDS_LEADERS_MONSTERS \
    CASE(1 , BASE_GAME      , MONSTERS       , monsters_eredin_1             , "Eredin: Bringer of Death"              , "monsters_eredin_1.png"            ) \
    CASE(2 , BASE_GAME      , MONSTERS       , monsters_eredin_2             , "Eredin: Commander of the Red Riders"   , "monsters_eredin_2.png"            ) \
    CASE(3 , BASE_GAME      , MONSTERS       , monsters_eredin_3             , "Eredin: Destroyer of Worlds"           , "monsters_eredin_3.png"            ) \
    CASE(4 , BASE_GAME      , MONSTERS       , monsters_eredin_4             , "Eredin: King of the Wild Hunt"         , "monsters_eredin_4.png"            ) \
    CASE(5 , HEARTS_OF_STONE, MONSTERS       , monsters_eredin_5             , "Eredin Bréacc Glas: The Treacherous"   , "monsters_eredin_5.png"            ) \

// id, expansion, deck, symbol, name, strength, is hero?, combat row, ability, texture name
#define NG_ENUM_CARDS_UNITS_MONSTERS \
    CASE(6 , BASE_GAME      , MONSTERS       , monsters_draug                , "Draug"                                 , 10, true  , MELEE          , NONE    , "monsters_draug.png"               ) \
    CASE(7 , BASE_GAME      , MONSTERS       , monsters_imlerith             , "Imlerith"                              , 10, true  , MELEE          , NONE    , "monsters_imlerith.png"            ) \
    CASE(8 , BASE_GAME      , MONSTERS       , monsters_kayran               , "Kayran"                                , 18, true  , MELEE_RANGED   , MORALE  , "monsters_kayran.png"              ) \
    CASE(9 , BASE_GAME      , MONSTERS       , monsters_leshan               , "Leshen"                                , 10, true  , RANGED         , NONE    , "monsters_leshan.png"              ) \
    CASE(10, BASE_GAME      , MONSTERS       , monsters_arachas_1            , "Arachas (1 of 3)"                      , 4 , false , MELEE          , MUSTER  , "monsters_arachas_1.png"           ) \
    CASE(11, BASE_GAME      , MONSTERS       , monsters_arachas_2            , "Arachas (2 of 3)"                      , 4 , false , MELEE          , MUSTER  , "monsters_arachas_2.png"           ) \
    CASE(12, BASE_GAME      , MONSTERS       , monsters_arachas_3            , "Arachas (3 of 3)"                      , 4 , false , MELEE          , MUSTER  , "monsters_arachas_3.png"           ) \
    CASE(13, BASE_GAME      , MONSTERS       , monsters_arachas_behemoth     , "Arachas Behemoth"                      , 6 , false , SIEGE          , MUSTER  , "monsters_arachas_behemoth.png"    ) \
    CASE(14, BASE_GAME      , MONSTERS       , monsters_botchling            , "Botchling"                             , 4 , false , MELEE          , NONE    , "monsters_botchling.png"           ) \
    CASE(15, BASE_GAME      , MONSTERS       , monsters_celaeno_harpy        , "Celaeno Harpy"                         , 2 , false , MELEE_RANGED   , AGILE   , "monsters_celaeno_harpy.png"       ) \
    CASE(16, BASE_GAME      , MONSTERS       , monsters_cockatrice           , "Cockatrice"                            , 2 , false , RANGED         , NONE    , "monsters_cockatrice.png"          ) \
    CASE(17, BASE_GAME      , MONSTERS       , monsters_crone_1              , "Crone: Brewess"                        , 6 , false , MELEE          , MUSTER  , "monsters_crone_1.png"             ) \
    CASE(18, BASE_GAME      , MONSTERS       , monsters_crone_2              , "Crone: Weavess"                        , 6 , false , MELEE          , MUSTER  , "monsters_crone_2.png"             ) \
    CASE(19, BASE_GAME      , MONSTERS       , monsters_crone_3              , "Crone: Whispess"                       , 6 , false , MELEE          , MUSTER  , "monsters_crone_3.png"             ) \
    CASE(20, BASE_GAME      , MONSTERS       , monsters_earth_elemental      , "Earth Elemental"                       , 6 , false , SIEGE          , NONE    , "monsters_earth_elemental.png"     ) \
    CASE(21, BASE_GAME      , MONSTERS       , monsters_endrega              , "Endrega"                               , 2 , false , RANGED         , NONE    , "monsters_endrega.png"             ) \
    CASE(22, BASE_GAME      , MONSTERS       , monsters_fiend                , "Fiend"                                 , 6 , false , MELEE          , NONE    , "monsters_fiend.png"               ) \
    CASE(23, BASE_GAME      , MONSTERS       , monsters_fire_elemental       , "Fire Elemental"                        , 6 , false , SIEGE          , NONE    , "monsters_fire_elemental.png"      ) \
    CASE(24, BASE_GAME      , MONSTERS       , monsters_foglet               , "Foglet"                                , 2 , false , MELEE          , NONE    , "monsters_foglet.png"              ) \
    CASE(25, BASE_GAME      , MONSTERS       , monsters_forktail             , "Forktail"                              , 5 , false , MELEE          , NONE    , "monsters_forktail.png"            ) \
    CASE(26, BASE_GAME      , MONSTERS       , monsters_frightener           , "Frightener"                            , 5 , false , MELEE          , NONE    , "monsters_frightener.png"          ) \
    CASE(27, BASE_GAME      , MONSTERS       , monsters_gargoyle             , "Gargoyle"                              , 2 , false , RANGED         , NONE    , "monsters_gargoyle.png"            ) \
    CASE(28, BASE_GAME      , MONSTERS       , monsters_ghoul_1              , "Ghoul (1 of 3)"                        , 1 , false , MELEE          , MUSTER  , "monsters_ghoul_1.png"             ) \
    CASE(29, BASE_GAME      , MONSTERS       , monsters_ghoul_2              , "Ghoul (2 of 3)"                        , 1 , false , MELEE          , MUSTER  , "monsters_ghoul_2.png"             ) \
    CASE(30, BASE_GAME      , MONSTERS       , monsters_ghoul_3              , "Ghoul (3 of 3)"                        , 1 , false , MELEE          , MUSTER  , "monsters_ghoul_3.png"             ) \
    CASE(31, BASE_GAME      , MONSTERS       , monsters_gravehag             , "Grave Hag"                             , 5 , false , RANGED         , NONE    , "monsters_gravehag.png"            ) \
    CASE(32, BASE_GAME      , MONSTERS       , monsters_gryffin              , "Griffin"                               , 5 , false , MELEE          , NONE    , "monsters_gryffin.png"             ) \
    CASE(33, BASE_GAME      , MONSTERS       , monsters_harpy                , "Harpy"                                 , 2 , false , MELEE_RANGED   , AGILE   , "monsters_harpy.png"               ) \
    CASE(34, BASE_GAME      , MONSTERS       , monsters_frost_giant          , "Ice Giant"                             , 5 , false , SIEGE          , NONE    , "monsters_frost_giant.png"         ) \
    CASE(34, HEARTS_OF_STONE, MONSTERS       , monsters_toad                 , "Toad"                                  , 7 , false , RANGED         , SCORCH  , "monsters_toad.png"                ) \
    CASE(36, BASE_GAME      , MONSTERS       , monsters_nekker_1             , "Nekker (1 of 3)"                       , 2 , false , MELEE          , MUSTER  , "monsters_nekker_1.png"            ) \
    CASE(37, BASE_GAME      , MONSTERS       , monsters_nekker_2             , "Nekker (2 of 3)"                       , 2 , false , MELEE          , MUSTER  , "monsters_nekker_2.png"            ) \
    CASE(38, BASE_GAME      , MONSTERS       , monsters_nekker_3             , "Nekker (3 of 3)"                       , 2 , false , MELEE          , MUSTER  , "monsters_nekker_3.png"            ) \
    CASE(39, BASE_GAME      , MONSTERS       , monsters_plague_maiden        , "Plague Maiden"                         , 5 , false , MELEE          , NONE    , "monsters_plague_maiden.png"       ) \
    CASE(40, BASE_GAME      , MONSTERS       , monsters_vamp_bruxa           , "Vampire: Bruxa"                        , 4 , false , MELEE          , MUSTER  , "monsters_vamp_bruxa.png"          ) \
    CASE(41, BASE_GAME      , MONSTERS       , monsters_vamp_ekimmara        , "Vampire: Ekimmara"                     , 4 , false , MELEE          , MUSTER  , "monsters_vamp_ekimmara.png"       ) \
    CASE(42, BASE_GAME      , MONSTERS       , monsters_vamp_fleder          , "Vampire: Fleder"                       , 4 , false , MELEE          , MUSTER  , "monsters_vamp_fleder.png"         ) \
    CASE(43, BASE_GAME      , MONSTERS       , monsters_vamp_garkain         , "Vampire: Garkain"                      , 4 , false , MELEE          , MUSTER  , "monsters_vamp_garkain.png"        ) \
    CASE(44, BASE_GAME      , MONSTERS       , monsters_vamp_katakan         , "Vampire: Katakan"                      , 5 , false , MELEE          , MUSTER  , "monsters_vamp_katakan.png"        ) \
    CASE(45, BASE_GAME      , MONSTERS       , monsters_werewolf             , "Werewolf"                              , 5 , false , MELEE          , NONE    , "monsters_werewolf.png"            ) \
    CASE(46, BASE_GAME      , MONSTERS       , monsters_wyvern               , "Wyvern"                                , 2 , false , RANGED         , NONE    , "monsters_wyvern.png"              ) \

#define NG_ENUM_CARDS_UNITS_NEUTRAL \
    CASE(47, BASE_GAME      , NEUTRAL        , neutral_ciri_1                , "Cirilla Fiona Elen Riannon"            , 15, true  , MELEE          , NONE    , "neutral_ciri_1.png"               ) \
    CASE(48, BASE_GAME      , NEUTRAL        , neutral_dandelion             , "Dandelion"                             , 2 , true  , MELEE          , MORALE  , "neutral_dandelion.png"            ) \
    CASE(49, BASE_GAME      , NEUTRAL        , neutral_geralt                , "Geralt of Rivia"                       , 15, true  , MELEE          , NONE    , "neutral_geralt.png"               ) \
    CASE(50, BASE_GAME      , NEUTRAL        , neutral_avallach              , "Mysterious Elf"                        , 0 , true  , MELEE          , SPY     , "neutral_avallach.png"             ) \
    CASE(51, BASE_GAME      , NEUTRAL        , neutral_triss                 , "Triss Merigold"                        , 7 , true  , MELEE          , NONE    , "neutral_triss.png"                ) \
    CASE(52, BASE_GAME      , NEUTRAL        , neutral_yennefer              , "Yennefer of Vengerberg"                , 7 , true  , RANGED         , MEDIC   , "neutral_yennefer.png"             ) \
    CASE(53, BASE_GAME      , NEUTRAL        , neutral_emiel                 , "Emiel Regis Rohellec Terzieff"         , 5 , false , MELEE          , NONE    , "neutral_emiel.png"                ) \
    CASE(54, BASE_GAME      , NEUTRAL        , neutral_vesemir               , "Vesemir"                               , 6 , false , MELEE          , NONE    , "neutral_vesemir.png"              ) \
    CASE(55, BASE_GAME      , NEUTRAL        , neutral_villen                , "Villentretenmerth"                     , 7 , false , MELEE          , SCORCH  , "neutral_villen.png"               ) \
    CASE(56, BASE_GAME      , NEUTRAL        , neutral_zoltan                , "Zoltan Chivay"                         , 5 , false , MELEE          , NONE    , "neutral_zoltan.png"               ) \
    CASE(57, HEARTS_OF_STONE, NEUTRAL        , neutral_cow                   , "Cow"                                   , 0 , false , RANGED         , NONE    , "neutral_cow.png"                  ) \
    CASE(58, HEARTS_OF_STONE, NEUTRAL        , neutral_gaunter_darkness_1    , "Gaunter O'Dimm: Darkness (1 of 3)"     , 4 , false , RANGED         , MUSTER  , "neutral_gaunter_darkness.png"     ) \
    CASE(59, HEARTS_OF_STONE, NEUTRAL        , neutral_gaunter_darkness_2    , "Gaunter O'Dimm: Darkness (2 of 3)"     , 4 , false , RANGED         , MUSTER  , "neutral_gaunter_darkness.png"     ) \
    CASE(60, HEARTS_OF_STONE, NEUTRAL        , neutral_gaunter_darkness_3    , "Gaunter O'Dimm: Darkness (3 of 3)"     , 4 , false , RANGED         , MUSTER  , "neutral_gaunter_darkness.png"     ) \
    CASE(61, HEARTS_OF_STONE, NEUTRAL        , neutral_gaunter               , "Gaunter O'Dimm"                        , 2 , false , SIEGE          , MUSTER  , "neutral_gaunter.png"              ) \
    CASE(62, HEARTS_OF_STONE, NEUTRAL        , neutral_olgierd               , "Olgierd von Everec"                    , 6 , false , MELEE_RANGED   , MORALE  , "neutral_olgierd.png"              )

#define NG_ENUM_CARDS_SPECIALS \
    CASE(63, BASE_GAME      , NEUTRAL        , special_frost                 , "Biting Frost (1 of 3)"                 , "special_frost.png"                ) \
    CASE(64, BASE_GAME      , NEUTRAL        , special_frost                 , "Biting Frost (2 of 3)"                 , "special_frost.png"                ) \
    CASE(65, BASE_GAME      , NEUTRAL        , special_frost                 , "Biting Frost (3 of 3)"                 , "special_frost.png"                ) \
    CASE(66, BASE_GAME      , NEUTRAL        , special_clearsky              , "Clear Weather (1 of 3)"                , "special_clearsky.png"             ) \
    CASE(67, BASE_GAME      , NEUTRAL        , special_clearsky              , "Clear Weather (2 of 3)"                , "special_clearsky.png"             ) \
    CASE(68, BASE_GAME      , NEUTRAL        , special_clearsky              , "Clear Weather (3 of 3)"                , "special_clearsky.png"             ) \
    CASE(69, BASE_GAME      , NEUTRAL        , special_horn                  , "Commander's Horn (1 of 3)"             , "special_horn.png"                 ) \
    CASE(70, BASE_GAME      , NEUTRAL        , special_horn                  , "Commander's Horn (2 of 3)"             , "special_horn.png"                 ) \
    CASE(71, BASE_GAME      , NEUTRAL        , special_horn                  , "Commander's Horn (3 of 3)"             , "special_horn.png"                 ) \
    CASE(72, BASE_GAME      , NEUTRAL        , special_dummy                 , "Decoy (1 of 3)"                        , "special_dummy.png"                ) \
    CASE(73, BASE_GAME      , NEUTRAL        , special_dummy                 , "Decoy (2 of 3)"                        , "special_dummy.png"                ) \
    CASE(74, BASE_GAME      , NEUTRAL        , special_dummy                 , "Decoy (3 of 3)"                        , "special_dummy.png"                ) \
    CASE(75, BASE_GAME      , NEUTRAL        , special_fog                   , "Impenetrable Fog (1 of 3)"             , "special_fog.png"                  ) \
    CASE(76, BASE_GAME      , NEUTRAL        , special_fog                   , "Impenetrable Fog (2 of 3)"             , "special_fog.png"                  ) \
    CASE(77, BASE_GAME      , NEUTRAL        , special_fog                   , "Impenetrable Fog (3 of 3)"             , "special_fog.png"                  ) \
    CASE(78, BASE_GAME      , NEUTRAL        , special_scorch                , "Scorch (1 of 3)"                       , "special_scorch.png"               ) \
    CASE(79, BASE_GAME      , NEUTRAL        , special_scorch                , "Scorch (2 of 3)"                       , "special_scorch.png"               ) \
    CASE(80, BASE_GAME      , NEUTRAL        , special_Scorch                , "Scorch (3 of 3)"                       , "special_Scorch.png"               ) \
    CASE(81, BASE_GAME      , NEUTRAL        , special_rain                  , "Torrential Rain (1 of 3)"              , "special_rain.png"                 ) \
    CASE(82, BASE_GAME      , NEUTRAL        , special_rain                  , "Torrential Rain (2 of 3)"              , "special_rain.png"                 ) \
    CASE(83, BASE_GAME      , NEUTRAL        , special_rain                  , "Torrential Rain (3 of 3)"              , "special_rain.png"                 ) \
    CASE(84, BLOOD_AND_WINE , NEUTRAL        , special_skellige_storm        , "Skellige Storm (1 of 3)"               , "special_skellige_storm.png"       ) \
    CASE(85, BLOOD_AND_WINE , NEUTRAL        , special_skellige_storm        , "Skellige Storm (2 of 3)"               , "special_skellige_storm.png"       ) \
    CASE(86, BLOOD_AND_WINE , NEUTRAL        , special_skellige_storm        , "Skellige Storm (3 of 3)"               , "special_skellige_storm.png"       )

#define NG_ENUM_CARDS_UNITS \
    NG_ENUM_CARDS_UNITS_MONSTERS \
    NG_ENUM_CARDS_UNITS_NEUTRAL

#include "ng/ng_card.hpp"

namespace ng
{
namespace cards
{
#define CASE(_p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _p9) \
    static const Card _p3 = Card::makeUnit(_p0, _p4, _p9, Card::EXPANSION_##_p1, Card::DECK_##_p2, { _p5, _p6, UnitCard::ABILITY_##_p8, UnitCard::ROW_##_p7 });
NG_ENUM_CARDS_UNITS
#undef CASE
}
}
