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

Card::Card()
    : Card(createTestCard())
{}

//m_tween_nudge    = Tween2(&easings::elasticOut , 1.0f);
//m_tween_offset      = Tween2(&easings::elasticOut , 1.0f);
//m_tween_rotation    = Tween1(&easings::expoOut    , 0.5f);
//m_tween_drop   = Tween1(&easings::bounceOut  , 0.5f);
//m_tween_opacity     = Tween1(&easings::expoOut    , 0.5f);

Card::Card(ng::Card const& card) :
        m_assets            {assetsFromCard(card)},
        m_card              {card},
        m_tween_nudge    {&easings::elasticOut , 1.0f},
        m_tween_rotation    {&easings::expoOut    , 0.5f},
        m_tween_drop   {&easings::bounceOut  , 0.5f},
        m_tween_opacity     {&easings::expoOut    , 0.5f}
{

}



//
// Card -> target
//

void Card::targetPosition(vec2f const& p)
{
    assert(m_is_layout_ready);

    if (m_position != p)
        m_tween_nudge.anim(m_position, p);
}

void Card::targetElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    if (m_elevation != f)
        m_tween_drop.anim(m_elevation, f);
}

void Card::targetRotation(float f)
{
    assert(m_is_layout_ready);

    if (m_rotation != f)
        m_tween_rotation.anim(m_rotation, f);
}

void Card::targetOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f); assert(f <= 1.0f);

    if (m_opacity != f)
        m_tween_opacity.anim(m_opacity,f);
}

//
// Card -> set
//

void Card::setCard(ng::Card const& card)
{
    m_assets = assetsFromCard(card);
    m_card = card;
}

void Card::setPosition(vec2f const& p)
{
    assert(m_is_layout_ready);

    m_position = p;
    m_tween_nudge.setToDst();
}

void Card::setElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_elevation = f;
    m_tween_drop.setToDst();
}

void Card::setRotation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_rotation = f;
    m_tween_rotation.setToDst();
}

void Card::setOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f); assert(f <= 1.0f);

    m_opacity = f;
    m_tween_opacity.setToDst();
}

//
// Card -> anim
//

void Card::animDrop()
{
    m_tween_drop.set(&easings::bounceOut  , 0.5f);
    m_tween_drop.anim(m_elevation, ELEVATION_DROP);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, {0.0f, 0.0f});
}

void Card::animPeek()
{
    m_tween_drop.set(&easings::expoOut, 0.5f);
    m_tween_drop.anim(m_elevation, ELEVATION_PEEK);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, {0.0f, 0.0f});
}

void Card::animGrab()
{
    m_tween_drop.set(&easings::expoOut, 0.5f);
    m_tween_drop.anim(m_elevation, ELEVATION_GRAB);
}

void Card::animNudge(vec2f const& p)
{
    m_tween_nudge.set(&easings::expoOut , 1.0f);
    m_tween_nudge.anim(m_position, p);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, m_offset * 0.75f);
}

void Card::animThrow(vec2f const& src, vec2f const& dst)
{
    m_position = src;

    m_tween_nudge.set(&easings::expoOut , 0.75f);
    m_tween_nudge.anim(src, dst);

    m_tween_drop.set(&easings::bounceOut , 1.5f);
    m_tween_drop.anim(ELEVATION_GRAB, ELEVATION_DROP);
}

void Card::layout(ut::rect const& bounds)
{
    auto b = bounds.anchorCCtoCC(bounds.fit((float)m_assets.artwork.width, (float)m_assets.artwork.height));
    layout(b.size());
}

void Card::layout(vec2 const& size)
{
    m_width     = size.x;
    m_height    = size.y;
    m_elevation = ELEVATION_DROP;
    m_opacity   = 1.0f;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

bool Card::update()
{
    assert(m_is_layout_ready);

    auto dt = GetFrameTime();
    auto b = false;

    if (m_tween_nudge    .update(dt)) { b = true; m_position  = m_tween_nudge.now(); }
    if (m_tween_rotation .update(dt)) { b = true; m_rotation  = m_tween_rotation.now(); }
    if (m_tween_drop     .update(dt)) { b = true; m_elevation = m_tween_drop.now(); }
    if (m_tween_opacity  .update(dt)) { b = true; m_opacity   = m_tween_opacity.now(); }

    return b;
}



void Card::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(rect());

    auto r = drawBounds();
    auto c = drawColor();
    auto r_pad = r.min.distance(r.max) / 20;

    auto outer = m_assets.color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.25f);

    auto shadow_sz  = ( (m_elevation - ELEVATION_DROP) / (ELEVATION_GRAB - ELEVATION_DROP) ) * r_pad;
    drawShadow(r, vec2(shadow_sz), shadow_sz + r_pad/2);

#if 0
    auto r_shadowl  = r.anchorLCtoRC(shadow_sz, r.height());
    auto r_shadowt  = r.anchorTCtoBC(r.width(), shadow_sz);
    auto r_shadowr  = r.anchorRCtoLC(shadow_sz, r.height());
    auto r_shadowb  = r.anchorBCtoTC(r.width(), shadow_sz);

    auto r_shadowtl  = r.anchorTLtoBR(shadow_sz, shadow_sz);
    auto r_shadowtr  = r.anchorTRtoBL(shadow_sz, shadow_sz);
    auto r_shadowbl  = r.anchorBLtoTR(shadow_sz, shadow_sz);
    auto r_shadowbr  = r.anchorBRtoTL(shadow_sz, shadow_sz);

    auto r_shadow_cast = r.withOffset({0, shadow_sz});

    VIRT_DEBUG(r_shadowl);
    VIRT_DEBUG(r_shadowr);
    VIRT_DEBUG(r_shadowt);
    VIRT_DEBUG(r_shadowb);

    VIRT_DEBUG(r_shadowtl);
    VIRT_DEBUG(r_shadowtr);
    VIRT_DEBUG(r_shadowbl);
    VIRT_DEBUG(r_shadowbr);

    // tl, bl, br, tr
    auto blk = BLACK;
    auto clr = blk;
    clr.a = 0;

    DrawRectangleGradientEx(torl(r_shadowl), clr, clr, blk, blk);
    DrawRectangleGradientEx(torl(r_shadowr), blk, blk, clr, clr);
    DrawRectangleGradientEx(torl(r_shadowt), clr, blk, blk, clr);
    DrawRectangleGradientEx(torl(r_shadowb), blk, clr, clr, blk);

    DrawRectangleGradientEx(torl(r_shadowtl), clr, clr, blk, clr);
    DrawRectangleGradientEx(torl(r_shadowtr), clr, blk, clr, clr);
    DrawRectangleGradientEx(torl(r_shadowbl), clr, clr, clr, blk);
    DrawRectangleGradientEx(torl(r_shadowbr), blk, clr, clr, clr);

    VIRT.drawRectangle(r_shadow_cast, colors::black.withNormalA(0.5));
#endif

    VIRT.drawTexturePro(m_assets.artwork, r, c);

    auto r_stats            = r.col(10, 7, {.h=3});
    auto r_stat_strength    = r_stats.row(3,0, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_row         = r_stats.row(3,1, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_ability     = r_stats.row(3,2, {.inner_pad=r_pad, .outer_pad=r_pad});



    VIRT.drawRectangle(r_stats, outer.withL(15).withA(0.85).toColor());

    if (m_card.isUnitCard())
    {
        auto& unit = m_card.asUnitCard();
        VIRT.drawTextBCtoBC(m_assets.font, r_stat_strength, PRINTER("%d", unit.strength), outer.toColor());
    }

    if (m_assets.hasRow())
        gfx::drawTextureFit(m_assets.row, r_stat_row, c);

    if (m_assets.hasAbility())
        gfx::drawTextureFit(m_assets.ability, r_stat_ability, c);

    VIRT.drawRectangleLines(r, 2.0f, outer.toColor());

    //rlPopMatrix();
    VIRT_DEBUG(r_stats);
    VIRT_DEBUG(r_stat_row);
    VIRT_DEBUG(r_stat_ability);
    VIRT_DEBUG(r_stat_strength);
}

RenderTexture2D Card::drawTexture()
{
    auto r = drawBounds();

    auto r2d = LoadRenderTexture(r.width(), r.height());

//    VIRT.beginTexture(r2d);
//    VIRT.drawLine(vec2(), vec2(100,100), 2.0f, colors::white);
//    VIRT.endTexture();

    return r2d;
}

RenderTexture2D Card::drawTexture(float width, float height)
{
    assert_impl();
}

Card Card::createTestCard()
{
    static rangen rg;

    auto id = (ng::CardID)rg.nexti((int)ng::CARD_COUNT_-1);

    return Card{ng::getCard(id)};
}

cardlist_t Card::createTestCards(size_t n)
{
    cardlist_t cards;
    for (size_t i = 0; i < n; ++i)
        cards.push_back(createTestCard());
    return cards;
}



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

Card::Assets Card::assetsFromCard(ng::Card const& card)
{
    Texture2D artwork = TEXTURES.get(PRINTER("data/cards/%s", card.filename.c_str()));

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