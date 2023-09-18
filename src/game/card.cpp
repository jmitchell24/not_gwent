//
// Created by james on 8/16/23.
//

#include "random.hpp"

#include "game/card.hpp"
#include "game/assets.hpp"
using namespace game;

//
// ng
//
#include "ng/ng_card_data.hpp"

//
// ut
//
#include <ut/random.hpp>
using namespace ut;

//
// std
//
#include <array>
#include <optional>
#include <unordered_set>
using namespace std;

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "gfx/gfx_easings.hpp"
#include "gfx/gfx_draw.hpp"
#include "conv.hpp"

using namespace gfx;

//
// Card -> Implementation
//

//Card::Card()
//    : Card(createTestCard())
//{}

//m_tween_nudge         = Tween2(&easings::elasticOut , 1.0f);
//m_tween_offset        = Tween2(&easings::elasticOut , 1.0f);
//m_tween_rotation      = Tween1(&easings::expoOut    , 0.5f);
//m_tween_drop          = Tween1(&easings::bounceOut  , 0.5f);
//m_tween_opacity       = Tween1(&easings::expoOut    , 0.5f);

//Card::Card(ng::Card const& card) :
//    m_assets        {assetsFromCard(card)},
//    m_card          {card}
////    m_tween_nudge   {&easings::elasticOut , 1.0f},
////    m_tween_drop    {&easings::bounceOut  , 0.5f}
//{ }

//
// Card -> set
//

//void Card::setPosition(vec2f const& p)
//{
//    assert(m_is_layout_ready);
//
//    m_position = p;
//    //m_tween_nudge.setToDst();
//}
//
//void Card::setElevation(float f)
//{
//    assert(m_is_layout_ready);
//    m_elevation = f;
//}

//
// Card -> anim
//

//void Card::animDrop()
//{
//    m_tween_drop.set(&easings::bounceOut  , 0.5f);
//    m_tween_drop.anim(m_elevation, ELEVATION_DROP);
//}
//
//void Card::animPeek()
//{
//    m_tween_drop.set(&easings::expoOut, 0.5f);
//    m_tween_drop.anim(m_elevation, ELEVATION_PEEK);
//}
//
//void Card::animGrab()
//{
//    m_tween_drop.set(&easings::expoOut, 0.5f);
//    m_tween_drop.anim(m_elevation, ELEVATION_GRAB);
//}
//
//void Card::animNudge(vec2f const& p)
//{
//    m_tween_nudge.set(&easings::expoOut , 1.0f);
//    m_tween_nudge.anim(m_position, p);
//}
//
//void Card::animThrow(vec2f const& src, vec2f const& dst)
//{
//    m_position = src;
//
//    m_tween_nudge.set(&easings::expoOut , 0.75f);
//    m_tween_nudge.anim(src, dst);
//
//    m_tween_drop.set(&easings::bounceOut , 1.5f);
//    m_tween_drop.anim(ELEVATION_GRAB, ELEVATION_DROP);
//}

//void Card::layout(ut::rect const& bounds)
//{
//    auto b = bounds.anchorCCtoCC(bounds.fit((float)m_assets.artwork.width, (float)m_assets.artwork.height));
//    layout(b.size());
//}
//
//void Card::layout(vec2 const& size)
//{
//    m_width     = size.x;
//    m_height    = size.y;
//    m_elevation = ELEVATION_DROP;
//
//#ifndef NDEBUG
//    m_is_layout_ready = true;
//#endif
//}

//bool Card::update()
//{
//    assert(m_is_layout_ready);
//
//    auto dt = GetFrameTime();
//    auto b = false;
//
//    if (m_tween_nudge    .update(dt)) { b = true; m_position  = m_tween_nudge.now(); }
//    if (m_tween_drop     .update(dt)) { b = true; m_elevation = m_tween_drop.now(); }
//
//    return b;
//}

bool Card::update(float dt)
{
    if (spring.isAtDst())
        return false;

    spring.update(dt);
    layout.setPosition3(spring.now());
    return true;
}

void Card::draw()
{
    assert(layout.isValid());

    auto r = layout.getRect();//getDrawRect();
    auto c = getDrawColor();
    auto r_pad = r.min.distance(r.max) / 20;

    auto outer = assets.color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.25f);

    //auto shadow_sz  = ( (layout.getElevation() - layout.ELEVATION_DROP) / (layout.ELEVATION_GRAB - layout.ELEVATION_DROP) ) * r_pad;
    //drawShadow(r, vec2(shadow_sz), shadow_sz + r_pad/2);

    VIRT.drawTexturePro(assets.artwork, r, c);

    auto r_stats            = r.col(10, 7, {.h=3});
    auto r_stat_strength    = r_stats.row(3,0, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_row         = r_stats.row(3,1, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_ability     = r_stats.row(3,2, {.inner_pad=r_pad, .outer_pad=r_pad});



    VIRT.drawRectangle(r_stats, outer.withL(15).withA(0.85).toColor());

    if (ng.isUnitCard())
    {
        auto& unit = ng.asUnitCard();
        VIRT.drawTextBCtoBC(assets.font, r_stat_strength, PRINTER("%d", unit.strength), outer.toColor());
    }

    if (assets.hasRow())
        gfx::drawTextureFit(assets.row, r_stat_row, c);

    if (assets.hasAbility())
        gfx::drawTextureFit(assets.ability, r_stat_ability, c);

    VIRT.drawRectangleLines(r, 2.0f, outer.toColor());

    //rlPopMatrix();

    DRECT_PUSH2(Card, r);
    DRECT1(r_stats);
    DRECT1(r_stat_row);
    DRECT1(r_stat_ability);
    DRECT1(r_stat_strength);
    DRECT_POP();



}

//RenderTexture2D Card::drawTexture()
//{
//    auto r = drawBounds();
//
//    auto r2d = LoadRenderTexture(r.width(), r.height());
//
////    VIRT.beginTexture(r2d);
////    VIRT.drawLine(vec2(), vec2(100,100), 2.0f, colors::white);
////    VIRT.endTexture();
//
//    return r2d;
//}
//
//RenderTexture2D Card::drawTexture(float width, float height)
//{
//    assert_impl();
//}





Texture2D abilityBadge(ng::UnitCard::Ability x)
{
    switch (x)
    {
        case ng::UnitCard::ABILITY_NONE     : return {};
        case ng::UnitCard::ABILITY_SPY      : return TEXTURES.get("data/board/card_ability_spy.png");
        case ng::UnitCard::ABILITY_BOND     : return TEXTURES.get("data/board/card_ability_bond.png");
        case ng::UnitCard::ABILITY_MORALE   : return TEXTURES.get("data/board/card_ability_morale.png");
        case ng::UnitCard::ABILITY_MEDIC    : return TEXTURES.get("data/board/card_ability_medic.png");
        case ng::UnitCard::ABILITY_AGILE    : return TEXTURES.get("data/board/card_ability_agile.png");
        case ng::UnitCard::ABILITY_MUSTER   : return TEXTURES.get("data/board/card_ability_muster.png");
        case ng::UnitCard::ABILITY_SCORCH   : return TEXTURES.get("data/board/card_ability_scorch.png");
        case ng::UnitCard::ABILITY_BERSERKER: return TEXTURES.get("data/board/card_ability_berserker.png");
        default: assert_case(ng::UnitCard::Ability);
    }

    return {};
}

Texture2D rowBadge(ng::UnitCard::Row x)
{
    switch (x)
    {
        case ng::UnitCard::ROW_MELEE        : return TEXTURES.get("data/board/combat_row_melee.png");
        case ng::UnitCard::ROW_MELEE_RANGED : return TEXTURES.get("data/board/combat_row_melee_ranged.png");
        case ng::UnitCard::ROW_RANGED       : return TEXTURES.get("data/board/combat_row_ranged.png");
        case ng::UnitCard::ROW_SIEGE        : return TEXTURES.get("data/board/combat_row_siege.png");
        default: assert_case(ng::UnitCard::Row);
    }

    return {};
}

Card::Assets Card::Assets::fromNgCard(ng::Card const& card)
{
    auto k = PRINTER("data/cards/%s", card.filename.c_str());
    Texture2D artwork = TEXTURES.get(k.c_str());

    if (card.isUnitCard())
    {
        auto& unit = card.asUnitCard();

        return
        {
            artwork,
            rowBadge(unit.row),
            abilityBadge(unit.ability),
            fonts::smallburgRegular64(),
            RANDOM.nextColor()
        };
    }

    return
    {
        artwork,
        {},
        {},
        fonts::smallburgRegular64(),
        RANDOM.nextColor()
    };
}

//Card Card::fromNgCard(ng::Card const& ng)
//{
//    Assets assets = Assets::fromNgCard(ng);
//    Card card;
//
//    return card;
//}