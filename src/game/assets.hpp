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

#define GAME_ENUM_TEXTURES_MISC \
    CASE(card_back, "data/board/card_back.png")

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

#define GAME_ENUM_TEXTURES_CARDS

#define GAME_ENUM_TEXTURES      \
    GAME_ENUM_TEXTURES_BADGES   \
    GAME_ENUM_TEXTURES_MISC     \
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