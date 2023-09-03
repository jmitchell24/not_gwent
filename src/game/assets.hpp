//
// Created by james on 8/29/23.
//
#pragma once

#include "game/asset_store.hpp"
#include "assert.hpp"

#define GAME_ENUM_FONTS \
    CASE(smallburgRegular32, "data/font/smallburg-Regular.ttf", 32) \
    CASE(smallburgRegular64, "data/font/smallburg-Regular.ttf", 64) \
    CASE(smallburgRegular128, "data/font/smallburg-Regular.ttf", 128)


#define GAME_ENUM_TEXTURES_BADGES \
    CASE(Alembic                , "data/ui/Alembic.png"       ) \
    CASE(Antennae               , "data/ui/Antennae.png"      ) \
    CASE(Anvil                  , "data/ui/Anvil.png"         ) \
    CASE(Apple                  , "data/ui/Apple.png"         ) \
    CASE(Arrow_Down_01          , "data/ui/Arrow_Down_01.png" ) \
    CASE(card_ability_agile     , "data/board/card_ability_agile.png") \
    CASE(card_ability_medic     , "data/board/card_ability_medic.png") \
    CASE(card_ability_morale    , "data/board/card_ability_morale.png") \
    CASE(card_ability_muster    , "data/board/card_ability_muster.png") \
    CASE(card_ability_spy       , "data/board/card_ability_spy.png") \
    CASE(combat_row_melee       , "data/board/combat_row_melee.png") \
    CASE(combat_row_ranged      , "data/board/combat_row_ranged.png") \
    CASE(combat_row_siege       , "data/board/combat_row_siege.png") \

#define GAME_ENUM_TEXTURES_CARDS \
    CASE(card_0,    "data/cards/0.png") \
    CASE(card_1001, "data/cards/1001.png") \
    CASE(card_1002, "data/cards/1002.png") \
    CASE(card_1003, "data/cards/1003.png") \
    CASE(card_1004, "data/cards/1004.png") \
    CASE(card_1005, "data/cards/1005.png") \
    CASE(card_100,  "data/cards/100.png") \
    CASE(card_101,  "data/cards/101.png") \
    CASE(card_102,  "data/cards/102.png") \
    CASE(card_103,  "data/cards/103.png") \
    CASE(card_105,  "data/cards/105.png") \
    CASE(card_106,  "data/cards/106.png") \
    CASE(card_107,  "data/cards/107.png") \
    CASE(card_108,  "data/cards/108.png") \
    CASE(card_109,  "data/cards/109.png") \
    CASE(card_10,   "data/cards/10.png") \
    CASE(card_111,  "data/cards/111.png") \
    CASE(card_112,  "data/cards/112.png") \
    CASE(card_113,  "data/cards/113.png") \
    CASE(card_114,  "data/cards/114.png") \
    CASE(card_115,  "data/cards/115.png") \
    CASE(card_116,  "data/cards/116.png") \
    CASE(card_11,   "data/cards/11.png") \
    CASE(card_120,  "data/cards/120.png") \
    CASE(card_121,  "data/cards/121.png") \
    CASE(card_125,  "data/cards/125.png") \
    CASE(card_126,  "data/cards/126.png") \
    CASE(card_127,  "data/cards/127.png") \
    CASE(card_12,   "data/cards/12.png") \
    CASE(card_130,  "data/cards/130.png") \
    CASE(card_131,  "data/cards/131.png") \
    CASE(card_132,  "data/cards/132.png") \
    CASE(card_135,  "data/cards/135.png") \
    CASE(card_136,  "data/cards/136.png") \
    CASE(card_13,   "data/cards/13.png") \
    CASE(card_140,  "data/cards/140.png") \
    CASE(card_141,  "data/cards/141.png") \
    CASE(card_145,  "data/cards/145.png") \
    CASE(card_146,  "data/cards/146.png") \
    CASE(card_14,   "data/cards/14.png") \
    CASE(card_150,  "data/cards/150.png") \
    CASE(card_151,  "data/cards/151.png") \
    CASE(card_152,  "data/cards/152.png") \
    CASE(card_15,   "data/cards/15.png") \
    CASE(card_160,  "data/cards/160.png") \
    CASE(card_16,   "data/cards/16.png") \
    CASE(card_170,  "data/cards/170.png") \
    CASE(card_175,  "data/cards/175.png") \
    CASE(card_17,   "data/cards/17.png") \
    CASE(card_18,   "data/cards/18.png") \
    CASE(card_19,   "data/cards/19.png") \
    CASE(card_1,    "data/cards/1.png") \
    CASE(card_2001, "data/cards/2001.png") \
    CASE(card_2002, "data/cards/2002.png") \
    CASE(card_2003, "data/cards/2003.png") \
    CASE(card_2004, "data/cards/2004.png") \
    CASE(card_2005, "data/cards/2005.png") \
    CASE(card_200,  "data/cards/200.png") \
    CASE(card_201,  "data/cards/201.png") \
    CASE(card_202,  "data/cards/202.png") \
    CASE(card_203,  "data/cards/203.png") \
    CASE(card_205,  "data/cards/205.png") \
    CASE(card_206,  "data/cards/206.png") \
    CASE(card_207,  "data/cards/207.png") \
    CASE(card_208,  "data/cards/208.png") \
    CASE(card_209,  "data/cards/209.png") \
    CASE(card_20,   "data/cards/20.png") \
    CASE(card_210,  "data/cards/210.png") \
    CASE(card_211,  "data/cards/211.png") \
    CASE(card_212,  "data/cards/212.png") \
    CASE(card_213,  "data/cards/213.png") \
    CASE(card_214,  "data/cards/214.png") \
    CASE(card_215,  "data/cards/215.png") \
    CASE(card_217,  "data/cards/217.png") \
    CASE(card_218,  "data/cards/218.png") \
    CASE(card_219,  "data/cards/219.png") \
    CASE(card_21,   "data/cards/21.png") \
    CASE(card_220,  "data/cards/220.png") \
    CASE(card_221,  "data/cards/221.png") \
    CASE(card_22,   "data/cards/22.png") \
    CASE(card_230,  "data/cards/230.png") \
    CASE(card_231,  "data/cards/231.png") \
    CASE(card_235,  "data/cards/235.png") \
    CASE(card_236,  "data/cards/236.png") \
    CASE(card_23,   "data/cards/23.png") \
    CASE(card_240,  "data/cards/240.png") \
    CASE(card_241,  "data/cards/241.png") \
    CASE(card_245,  "data/cards/245.png") \
    CASE(card_246,  "data/cards/246.png") \
    CASE(card_247,  "data/cards/247.png") \
    CASE(card_248,  "data/cards/248.png") \
    CASE(card_250,  "data/cards/250.png") \
    CASE(card_251,  "data/cards/251.png") \
    CASE(card_252,  "data/cards/252.png") \
    CASE(card_255,  "data/cards/255.png") \
    CASE(card_260,  "data/cards/260.png") \
    CASE(card_261,  "data/cards/261.png") \
    CASE(card_265,  "data/cards/265.png") \
    CASE(card_2,    "data/cards/2.png") \
    CASE(card_3001, "data/cards/3001.png") \
    CASE(card_3002, "data/cards/3002.png") \
    CASE(card_3003, "data/cards/3003.png") \
    CASE(card_3004, "data/cards/3004.png") \
    CASE(card_3005, "data/cards/3005.png") \
    CASE(card_300,  "data/cards/300.png") \
    CASE(card_301,  "data/cards/301.png") \
    CASE(card_302,  "data/cards/302.png") \
    CASE(card_303,  "data/cards/303.png") \
    CASE(card_305,  "data/cards/305.png") \
    CASE(card_306,  "data/cards/306.png") \
    CASE(card_307,  "data/cards/307.png") \
    CASE(card_308,  "data/cards/308.png") \
    CASE(card_309,  "data/cards/309.png") \
    CASE(card_310,  "data/cards/310.png") \
    CASE(card_311,  "data/cards/311.png") \
    CASE(card_312,  "data/cards/312.png") \
    CASE(card_313,  "data/cards/313.png") \
    CASE(card_320,  "data/cards/320.png") \
    CASE(card_321,  "data/cards/321.png") \
    CASE(card_322,  "data/cards/322.png") \
    CASE(card_325,  "data/cards/325.png") \
    CASE(card_326,  "data/cards/326.png") \
    CASE(card_330,  "data/cards/330.png") \
    CASE(card_331,  "data/cards/331.png") \
    CASE(card_332,  "data/cards/332.png") \
    CASE(card_335,  "data/cards/335.png") \
    CASE(card_336,  "data/cards/336.png") \
    CASE(card_337,  "data/cards/337.png") \
    CASE(card_340,  "data/cards/340.png") \
    CASE(card_341,  "data/cards/341.png") \
    CASE(card_342,  "data/cards/342.png") \
    CASE(card_343,  "data/cards/343.png") \
    CASE(card_344,  "data/cards/344.png") \
    CASE(card_350,  "data/cards/350.png") \
    CASE(card_351,  "data/cards/351.png") \
    CASE(card_352,  "data/cards/352.png") \
    CASE(card_355,  "data/cards/355.png") \
    CASE(card_360,  "data/cards/360.png") \
    CASE(card_365,  "data/cards/365.png") \
    CASE(card_366,  "data/cards/366.png") \
    CASE(card_367,  "data/cards/367.png") \
    CASE(card_368,  "data/cards/368.png") \
    CASE(card_3,    "data/cards/3.png") \
    CASE(card_4001, "data/cards/4001.png") \
    CASE(card_4002, "data/cards/4002.png") \
    CASE(card_4003, "data/cards/4003.png") \
    CASE(card_4004, "data/cards/4004.png") \
    CASE(card_4005, "data/cards/4005.png") \
    CASE(card_400,  "data/cards/400.png") \
    CASE(card_401,  "data/cards/401.png") \
    CASE(card_402,  "data/cards/402.png") \
    CASE(card_403,  "data/cards/403.png") \
    CASE(card_405,  "data/cards/405.png") \
    CASE(card_407,  "data/cards/407.png") \
    CASE(card_410,  "data/cards/410.png") \
    CASE(card_413,  "data/cards/413.png") \
    CASE(card_415,  "data/cards/415.png") \
    CASE(card_417,  "data/cards/417.png") \
    CASE(card_420,  "data/cards/420.png") \
    CASE(card_423,  "data/cards/423.png") \
    CASE(card_425,  "data/cards/425.png") \
    CASE(card_427,  "data/cards/427.png") \
    CASE(card_430,  "data/cards/430.png") \
    CASE(card_433,  "data/cards/433.png") \
    CASE(card_435,  "data/cards/435.png") \
    CASE(card_437,  "data/cards/437.png") \
    CASE(card_440,  "data/cards/440.png") \
    CASE(card_443,  "data/cards/443.png") \
    CASE(card_445,  "data/cards/445.png") \
    CASE(card_447,  "data/cards/447.png") \
    CASE(card_450,  "data/cards/450.png") \
    CASE(card_451,  "data/cards/451.png") \
    CASE(card_452,  "data/cards/452.png") \
    CASE(card_453,  "data/cards/453.png") \
    CASE(card_455,  "data/cards/455.png") \
    CASE(card_456,  "data/cards/456.png") \
    CASE(card_457,  "data/cards/457.png") \
    CASE(card_460,  "data/cards/460.png") \
    CASE(card_461,  "data/cards/461.png") \
    CASE(card_462,  "data/cards/462.png") \
    CASE(card_463,  "data/cards/463.png") \
    CASE(card_464,  "data/cards/464.png") \
    CASE(card_470,  "data/cards/470.png") \
    CASE(card_471,  "data/cards/471.png") \
    CASE(card_472,  "data/cards/472.png") \
    CASE(card_475,  "data/cards/475.png") \
    CASE(card_476,  "data/cards/476.png") \
    CASE(card_477,  "data/cards/477.png") \
    CASE(card_478,  "data/cards/478.png") \
    CASE(card_4,    "data/cards/4.png") \
    CASE(card_5001, "data/cards/5001.png") \
    CASE(card_5002, "data/cards/5002.png") \
    CASE(card_500,  "data/cards/500.png") \
    CASE(card_501,  "data/cards/501.png") \
    CASE(card_502,  "data/cards/502.png") \
    CASE(card_503,  "data/cards/503.png") \
    CASE(card_504,  "data/cards/504.png") \
    CASE(card_505,  "data/cards/505.png") \
    CASE(card_506,  "data/cards/506.png") \
    CASE(card_507,  "data/cards/507.png") \
    CASE(card_508,  "data/cards/508.png") \
    CASE(card_509,  "data/cards/509.png") \
    CASE(card_510,  "data/cards/510.png") \
    CASE(card_511,  "data/cards/511.png") \
    CASE(card_512,  "data/cards/512.png") \
    CASE(card_513,  "data/cards/513.png") \
    CASE(card_514,  "data/cards/514.png") \
    CASE(card_515,  "data/cards/515.png") \
    CASE(card_516,  "data/cards/516.png") \
    CASE(card_517,  "data/cards/517.png") \
    CASE(card_518,  "data/cards/518.png") \
    CASE(card_519,  "data/cards/519.png") \
    CASE(card_520,  "data/cards/520.png") \
    CASE(card_521,  "data/cards/521.png") \
    CASE(card_522,  "data/cards/522.png") \
    CASE(card_523,  "data/cards/523.png") \
    CASE(card_524,  "data/cards/524.png") \
    CASE(card_525,  "data/cards/525.png") \
    CASE(card_526,  "data/cards/526.png") \
    CASE(card_527,  "data/cards/527.png") \
    CASE(card_5,    "data/cards/5.png") \
    CASE(card_6,    "data/cards/6.png") \
    CASE(card_7,    "data/cards/7.png") \
    CASE(card_8,    "data/cards/8.png") \
    CASE(card_9,    "data/cards/9.png")

#define GAME_ENUM_TEXTURES      \
    GAME_ENUM_TEXTURES_BADGES   \
    GAME_ENUM_TEXTURES_CARDS


namespace game::textures
{
#define CASE(x_, y_) static inline auto x_() { return game::TEXTURES.get(y_); }
GAME_ENUM_TEXTURES
#undef CASE

}

namespace game::fonts
{
#define CASE(x_, y_, z_) static inline auto x_() { return game::FONTS.get({y_, z_}); }
GAME_ENUM_FONTS
#undef CASE
}

namespace game
{
    enum CardTextureID
    {
#define CASE(x_, y_) CARD_TEXTURE_ID_##x_,
GAME_ENUM_TEXTURES_CARDS
#undef CASE
        _CARD_TEXTURE_ID_COUNT
    };

    static inline Texture2D getCardTexture(CardTextureID id)
    {
        switch (id)
        {
#define CASE(x_, y_) case CARD_TEXTURE_ID_##x_: return game::textures::x_();
GAME_ENUM_TEXTURES_CARDS
#undef CASE
            default:assert_case(CardTextureID);
        }

        return game::asset_loaders::Texture2DLoader::errorValue();
    }



    enum BadgeTextureID
    {
#define CASE(x_, y_) BADGE_TEXTURE_ID_##x_,
GAME_ENUM_TEXTURES_BADGES
#undef CASE
        _BADGE_TEXTURE_ID_COUNT
    };

    static inline Texture2D getBadgeTexture(BadgeTextureID id)
    {
        switch (id)
        {
#define CASE(x_, y_) case BADGE_TEXTURE_ID_##x_: return game::textures::x_();
GAME_ENUM_TEXTURES_BADGES
#undef CASE
            default:assert_case(BadgeTextureID);
        }

        return game::asset_loaders::Texture2DLoader::errorValue();
    }
}