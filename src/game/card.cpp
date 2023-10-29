//
// Created by james on 8/16/23.
//

#include "game/card.hpp"
#include "game/card_tank.hpp"
#include "game/asset/assets.hpp"
using namespace game;

//
// ng
//
#include "ng/ng_card_data.hpp"


//
// ut
//
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
    {
        m_is_animated = false;
        return false;
    }

    spring.update(dt);
    layout.setPosition3(spring.now());
    m_is_animated = true;
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

    drawTexturePro(assets.card, r, colors::white);
    drawTexturePro(assets.artwork,
                        r.anchorCCtoCC(r.size()*.85), colors::white);




    auto r_stats            = r.col(10, 7, {.h=3});
    auto r_stat_strength    = r_stats.row(3,0, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_row         = r_stats.row(3,1, {.inner_pad=r_pad, .outer_pad=r_pad});
    auto r_stat_ability     = r_stats.row(3,2, {.inner_pad=r_pad, .outer_pad=r_pad});


    if (ng.isUnitCard())
    {
        auto& unit = ng.asUnitCard();
        drawTextBCtoBC(assets.font, r_stat_strength, PRINTER("%d", unit.strength), outer.toColor());
    }

    if (assets.hasRow())
        drawTextureFit(assets.row, r_stat_row, c);

    if (assets.hasAbility())
        drawTextureFit(assets.ability, r_stat_ability, c);

    DRECT_PUSH2(Card, r);
    DRECT1(r_stats);
    DRECT1(r_stat_row);
    DRECT1(r_stat_ability);
    DRECT1(r_stat_strength);
    DRECT_POP();
}

void Card::drawDebug()
{

}

void Card::move2(ut::vec2 const& p)
{
    spring.setNow(layout.getPosition3());
    spring.setDst({p.x, p.y, layout.getElevation()});
    m_is_animated = true;
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





Texture2D abilityBadge(ng::AbilityType x)
{
    switch (x)
    {
        case ng::ABILITY_NONE     : return {};
        case ng::ABILITY_SPY      : return TEXTURES.get("data/board/card_ability_spy.png");
        case ng::ABILITY_BOND     : return TEXTURES.get("data/board/card_ability_bond.png");
        case ng::ABILITY_MORALE   : return TEXTURES.get("data/board/card_ability_morale.png");
        case ng::ABILITY_MEDIC    : return TEXTURES.get("data/board/card_ability_medic.png");
        case ng::ABILITY_AGILE    : return TEXTURES.get("data/board/card_ability_agile.png");
        case ng::ABILITY_MUSTER   : return TEXTURES.get("data/board/card_ability_muster.png");
        case ng::ABILITY_SCORCH   : return TEXTURES.get("data/board/card_ability_scorch.png");
        case ng::ABILITY_BERSERKER: return TEXTURES.get("data/board/card_ability_berserker.png");
        default: nopath_case(ng::AbilityType);
    }

    return {};
}

Texture2D rowBadge(ng::RowFlag x)
{
    switch (x)
    {
        case ng::ROW_MELEE        : return TEXTURES.get("data/board/combat_row_melee.png");
        case ng::ROW_MELEE_RANGED : return TEXTURES.get("data/board/combat_row_melee_ranged.png");
        case ng::ROW_RANGED       : return TEXTURES.get("data/board/combat_row_ranged.png");
        case ng::ROW_SIEGE        : return TEXTURES.get("data/board/combat_row_siege.png");
        default: nopath_case(ng::RowFlag_);
    }

    return {};
}

static color nextColor()
{
    static size_t counter=0;

    auto hue = float(counter++) * 100.0f + 120.0f;
    hue = fmodf(hue, 360.0f);

    return color(color::hsluv{hue, 80.0f, 80.0f, 1.0f});
}

Card::Assets Card::Assets::fromNgCard(ng::Card const& card)
{
    auto k = PRINTER("data/cards/%s", card.filename.c_str());
    Texture2D artwork = CARD_TEXTURES.get(k.c_str());

    if (card.isUnitCard())
    {
        auto& unit = card.asUnitCard();

        return
        {
            artwork,
            game::textures::card_back_neutral(),
            rowBadge(unit.row),
            abilityBadge(unit.ability),
            fonts::smallburgRegular64(),
            nextColor()
        };
    }

    return
    {
        artwork,
        game::textures::card_back_neutral(),
        {},
        {},
        fonts::smallburgRegular64(),
        nextColor()
    };
}

//Card Card::fromNgCard(ng::Card const& ng)
//{
//    Assets assets = Assets::fromNgCard(ng);
//    Card card;
//
//    return card;
//}

//
// CardRef -> Implementation
//

bool CardRef::inTank() const { return TANK.hasCard(id); }

Card& CardRef::operator*() { return TANK.getCard(id); }
Card const& CardRef::operator*() const { return TANK.getCard(id); }

Card* CardRef::operator->() { return &TANK.getCard(id); }
Card const* CardRef::operator->() const { return &TANK.getCard(id); }