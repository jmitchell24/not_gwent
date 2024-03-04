//
// Created by james on 8/29/23.
//
#pragma once

#include "res/res_store.hpp"

#include <ut/check.hpp>

#define RES_EXPAND_FONTS(CASE) \
    CASE(smallburgRegular32 , "data/font/smallburg-Regular.ttf", 32 ) \
    CASE(smallburgRegular64 , "data/font/smallburg-Regular.ttf", 64 ) \
    CASE(smallburgRegular128, "data/font/smallburg-Regular.ttf", 128) \
    CASE(steelfish128       , "data/font/steelfish.otf"        , 128)

#define RES_EXPAND_SVG(CASE) \
    CASE(tiger, "data/svg/tiger.svg", 512, 512)

#define RES_EXPAND_TEXTURES_BADGES(CASE) \
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

#define RES_EXPAND_TEXTURES_CARD_BACKS(CASE) \
    CASE(card_back_graveyard      ,     "data/board/card_back_graveyard.png") \
    CASE(card_back_monster        ,     "data/board/card_back_monster.png") \
    CASE(card_back_neutral        ,     "data/board/card_back_neutral.png") \
    CASE(card_back_nilfgaard      ,     "data/board/card_back_nilfgaard.png") \
    CASE(card_back_northern_realms,     "data/board/card_back_northern_realms.png") \
    CASE(card_back_questionmark   ,     "data/board/card_back_questionmark.png") \
    CASE(card_back_scoiatael      ,     "data/board/card_back_scoiatael.png") \
    CASE(card_back_skellige       ,     "data/board/card_back_skellige.png") \
    CASE(card_back_syndicate      ,     "data/board/card_back_syndicate.png")

#define RES_EXPAND_TEXTURES_AVATARS(CASE) \
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


#define RES_EXPAND_TEXTURES_MISC(CASE) \
    CASE(card_blank_sm, "data/board/card_blank_sm.png")

#define RES_EXPAND_TEXTURES(CASE)           \
    RES_EXPAND_TEXTURES_BADGES      (CASE)  \
    RES_EXPAND_TEXTURES_CARD_BACKS  (CASE)  \
    RES_EXPAND_TEXTURES_AVATARS     (CASE)  \
    RES_EXPAND_TEXTURES_MISC        (CASE)

#define RES_CASE_TEXTURES(x_, y_)       static inline auto x_() { return res::TEXTURES.get(y_); }
#define RES_CASE_SVG(x_, y_, z_, w_)    static inline auto x_() { return res::SVG.get({y_, z_, w_}); }
#define RES_CASE_FONTS(x_, y_, z_)      static inline auto x_() { return res::FONTS.get({y_, z_}); }

#define RES_CASE_ENUM_CARD_BACKS(x_, y_) CARD_BACKS_ID_##x_,
#define RES_CASE_ENUM_AVATARS(x_, y_)    AVATARS_ID_##x_,
#define RES_CASE_ENUM_BADGES(x_, y_)     BADGES_ID_##x_,

namespace res::textures
{
    RES_EXPAND_TEXTURES(RES_CASE_TEXTURES)
}

namespace res::svg
{
    RES_EXPAND_SVG(RES_CASE_SVG)
}

namespace res::fonts
{
    RES_EXPAND_FONTS(RES_CASE_FONTS)
}

#if 0
namespace res
{
    enum CardBackTextureID
    {

        RES_EXPAND_TEXTURES_CARD_BACKS(RES_CASE_ENUM_CARD_BACKS)
        _CARD_TEXTURE_ID_COUNT
    };

    static inline Texture2D getCardBackTexture(CardBackTextureID id)
    {
        switch (id)
        {
#define CASE(x_, y_) case CARD_TEXTURE_ID_##x_: return res::textures::x_();
RES_EXPAND_TEXTURES_CARD_BACKS(CASE)
#undef CASE
            default:nopath_case(CardBackTextureID);
        }

        return res::loaders::Texture2DLoader::errorValue();
    }

    enum AvatarTextureID
    {
#define CASE(x_, y_) AVATAR_ID_##x_,
RES_EXPAND_TEXTURES_AVATARS(CASE)
#undef CASE
        _AVATAR_TEXTURE_ID_COUNT
    };

    enum BadgeTextureID
    {
#define CASE(x_, y_) BADGE_TEXTURE_ID_##x_,
RES_EXPAND_TEXTURES_BADGES(CASE)
#undef CASE
        _BADGE_TEXTURE_ID_COUNT
    };

    static inline Texture2D getBadgeTexture(BadgeTextureID id)
    {
        switch (id)
        {
#define CASE(x_, y_) case BADGE_TEXTURE_ID_##x_: return res::textures::x_();
RES_EXPAND_TEXTURES_BADGES(CASE)
#undef CASE
            default:nopath_case(BadgeTextureID);
        }

        return res::loaders::Texture2DLoader::errorValue();
    }
}
#endif