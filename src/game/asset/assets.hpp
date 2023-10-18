//
// Created by james on 8/29/23.
//
#pragma once

#include "game/asset/asset_store.hpp"

#include <ut/check.hpp>

#define GAME_ENUM_FONTS \
    CASE(smallburgRegular32 , "data/font/smallburg-Regular.ttf", 32 ) \
    CASE(smallburgRegular64 , "data/font/smallburg-Regular.ttf", 64 ) \
    CASE(smallburgRegular128, "data/font/smallburg-Regular.ttf", 128) \
    CASE(steelfish128       , "data/font/steelfish.otf"        , 128)

#define GAME_ENUM_SVG \
    CASE(tiger, "data/svg/tiger.svg", 512, 512)

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

#define GAME_ENUM_TEXTURES_CARD_BACKS \
    CASE(card_back_graveyard      ,     "data/board/card_back_graveyard.png") \
    CASE(card_back_monster        ,     "data/board/card_back_monster.png") \
    CASE(card_back_neutral        ,     "data/board/card_back_neutral.png") \
    CASE(card_back_nilfgaard      ,     "data/board/card_back_nilfgaard.png") \
    CASE(card_back_northern_realms,     "data/board/card_back_northern_realms.png") \
    CASE(card_back_questionmark   ,     "data/board/card_back_questionmark.png") \
    CASE(card_back_scoiatael      ,     "data/board/card_back_scoiatael.png") \
    CASE(card_back_skellige       ,     "data/board/card_back_skellige.png") \
    CASE(card_back_syndicate      ,     "data/board/card_back_syndicate.png")

#define GAME_ENUM_TEXTURES_AVATARS \
    CASE(avatar_caranthir,      "data/avatars/avatar_caranthir.png") \
    CASE(avatar_dandelion,      "data/avatars/avatar_dandelion.png") \
    CASE(avatar_giant,          "data/avatars/avatar_giant.png") \
    CASE(avatar_triss,          "data/avatars/avatar_triss.png") \
    CASE(avatar_ciri,           "data/avatars/avatar_ciri.png") \
    CASE(avatar_emhyr,          "data/avatars/avatar_emhyr.png") \
    CASE(avatar_imlerith,       "data/avatars/avatar_imlerith.png") \
    CASE(avatar_vesemir,        "data/avatars/avatar_vesemir.png") \
    CASE(avatar_crache,         "data/avatars/avatar_crache.png") \
    CASE(avatar_eredin,         "data/avatars/avatar_eredin.png") \
    CASE(avatar_leshen,         "data/avatars/avatar_leshen.png") \
    CASE(avatar_yenn,           "data/avatars/avatar_yenn.png")


#define GAME_ENUM_TEXTURES_MISC \
    CASE(card_blank_sm, "data/board/card_blank_sm.png")



#define GAME_ENUM_TEXTURES        \
    GAME_ENUM_TEXTURES_BADGES     \
    GAME_ENUM_TEXTURES_CARD_BACKS \
    GAME_ENUM_TEXTURES_AVATARS \
    GAME_ENUM_TEXTURES_MISC

namespace game::textures
{
#define CASE(x_, y_) static inline auto x_() { return game::TEXTURES.get(y_); }
GAME_ENUM_TEXTURES
#undef CASE

}

namespace game::svg
{
#define CASE(x_, y_, z_, w_) static inline auto x_() { return game::SVG.get({y_, z_, w_}); }
    GAME_ENUM_SVG
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
    enum CardBackTextureID
    {
#define CASE(x_, y_) CARD_TEXTURE_ID_##x_,
GAME_ENUM_TEXTURES_CARD_BACKS
#undef CASE
        _CARD_TEXTURE_ID_COUNT
    };

    static inline Texture2D getCardBackTexture(CardBackTextureID id)
    {
        switch (id)
        {
#define CASE(x_, y_) case CARD_TEXTURE_ID_##x_: return game::textures::x_();
GAME_ENUM_TEXTURES_CARD_BACKS
#undef CASE
            default:nopath_case(CardBackTextureID);
        }

        return game::asset_loaders::Texture2DLoader::errorValue();
    }

    enum AvatarTextureID
    {
#define CASE(x_, y_) BADGE_TEXTURE_ID_##x_,
GAME_ENUM_TEXTURES_AVATARS
#undef CASE
        _AVATAR_TEXTURE_ID_COUNT
    };

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
            default:nopath_case(BadgeTextureID);
        }

        return game::asset_loaders::Texture2DLoader::errorValue();
    }
}