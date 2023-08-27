//
// Created by james on 8/16/23.
//

#include "random.hpp"

#include "game/card.hpp"
using namespace game;

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
#include "conv.hpp"

using namespace gfx;



//
// Card -> Implementation
//

Card::Card()
    : Card(createTestCard())
{}

//m_tween_position    = Tween2(&easings::elasticOut , 1.0f);
//m_tween_offset      = Tween2(&easings::elasticOut , 1.0f);
//m_tween_rotation    = Tween1(&easings::expoOut    , 0.5f);
//m_tween_elevation   = Tween1(&easings::bounceOut  , 0.5f);
//m_tween_opacity     = Tween1(&easings::expoOut    , 0.5f);

Card::Card(color const& color, Font const& font, Texture2D const& texture) :
    m_color {color}, m_font {font}, m_texture {texture},
    m_tween_position  {&easings::elasticOut , 1.0f},
    m_tween_offset    {&easings::elasticOut , 1.0f},
    m_tween_rotation  {&easings::expoOut    , 0.5f},
    m_tween_elevation {&easings::bounceOut  , 0.5f},
    m_tween_opacity   {&easings::expoOut    , 0.5f}
{
    assert(IsFontReady(font));
    assert(IsTextureReady(texture));
}

//
// Card -> target
//

void Card::targetPosition(vec2f const& p)
{
    assert(m_is_layout_ready);

    if (m_position != p)
        m_tween_position.anim(m_position, p);
}

void Card::targetOffset(vec2f const& p)
{
    assert(m_is_layout_ready);

    if (m_offset != p)
        m_tween_offset.anim(m_offset, p);
}

void Card::targetElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    if (m_elevation != f)
        m_tween_elevation.anim(m_elevation, f);
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

void Card::setPosition(vec2f const& p)
{
    assert(m_is_layout_ready);

    m_position = p;
    m_tween_position.setToDst();
}

void Card::setOffset(vec2f const& p)
{
    assert(m_is_layout_ready);

    m_offset = p;
    m_tween_offset.setToDst();
}

void Card::setElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_elevation = f;
    m_tween_elevation.setToDst();
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
    m_tween_elevation.set(&easings::bounceOut  , 0.5f);
    m_tween_elevation.anim(m_elevation, ELEVATION_DROP);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, {0.0f, 0.0f});
}

void Card::animPeek()
{
    m_tween_elevation.set(&easings::expoOut, 0.5f);
    m_tween_elevation.anim(m_elevation, ELEVATION_PEEK);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, {0.0f, 0.0f});
}

void Card::animGrab()
{
    m_tween_elevation.set(&easings::expoOut, 0.5f);
    m_tween_elevation.anim(m_elevation, ELEVATION_GRAB);

    m_tween_position.setToDst();
    m_tween_offset.setToDst();

    m_position += m_offset;
    m_offset.set(0);
}

void Card::animNudge(vec2f const& p)
{
    m_tween_position.set(&easings::expoOut , 1.0f);
    m_tween_position.anim(m_position, p);

//    m_tween_offset.set(&easings::expoOut , 0.5f);
//    m_tween_offset.anim(m_offset, m_offset * 0.75f);
}

void Card::animPlace(vec2f const& p)
{
    m_tween_position.set(&easings::expoOut , 0.5f);
    m_tween_position.anim(m_position, p);
}

void Card::layout(ut::rect const& bounds)
{
    auto b = bounds.anchorCCtoCC(bounds.fit((float)m_texture.width, (float)m_texture.height));
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

void Card::update()
{
    assert(m_is_layout_ready);

    auto dt = GetFrameTime();

    if (m_tween_position     .update(dt)) m_position  = m_tween_position .now();
    if (m_tween_offset       .update(dt)) m_offset    = m_tween_offset   .now();
    if (m_tween_rotation     .update(dt)) m_rotation  = m_tween_rotation .now();
    if (m_tween_elevation    .update(dt)) m_elevation = m_tween_elevation.now();
    if (m_tween_opacity      .update(dt)) m_opacity   = m_tween_opacity  .now();
}



void Card::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(rect());

    auto r = drawBounds();
    auto c = drawColor();
    auto r_pad = r.min.distance(r.max) / 20;

    auto outer = m_color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.25f);


    auto shadow_sz  = ( (m_elevation - ELEVATION_DROP) / (ELEVATION_GRAB - ELEVATION_DROP) ) * r_pad;

    auto r_shadowl  = r.anchorLCtoRC(shadow_sz, r.height());
    auto r_shadowt  = r.anchorTCtoBC(r.width(), shadow_sz);
    auto r_shadowr  = r.anchorRCtoLC(shadow_sz, r.height());
    auto r_shadowb  = r.anchorBCtoTC(r.width(), shadow_sz);

    auto r_shadowtl  = r.anchorTLtoBR(shadow_sz, shadow_sz);
    auto r_shadowtr  = r.anchorTRtoBL(shadow_sz, shadow_sz);
    auto r_shadowbl  = r.anchorBLtoTR(shadow_sz, shadow_sz);
    auto r_shadowbr  = r.anchorBRtoTL(shadow_sz, shadow_sz);

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


    VIRT.drawTexturePro(m_texture, r, c);
    //VIRT.drawRectangle(r, inner.toColor());






    VIRT.drawRectangle(r.col(10, 7, {.h=3}), outer.withL(15).withA(0.85).toColor());

    VIRT.drawTextBRtoBR(m_font, r.col(5 , 4, {.outer_pad=r_pad}), "10"_sv, outer.toColor());
    VIRT.drawTextTLtoTL(m_font, r.col(10, 8, {.outer_pad=r_pad}), "ranged"_sv, outer.toColor());
    VIRT.drawTextTLtoTL(m_font, r.col(10, 9, {.outer_pad=r_pad}), "morale"_sv, outer.toColor());

    VIRT.drawRectangleLines(r, 2.0f, outer.toColor());



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
    static array<char const*, 225> filenames
    {
        "data/cards/0.png",
        "data/cards/1001.png",
        "data/cards/1002.png",
        "data/cards/1003.png",
        "data/cards/1004.png",
        "data/cards/1005.png",
        "data/cards/100.png",
        "data/cards/101.png",
        "data/cards/102.png",
        "data/cards/103.png",
        "data/cards/105.png",
        "data/cards/106.png",
        "data/cards/107.png",
        "data/cards/108.png",
        "data/cards/109.png",
        "data/cards/10.png",
        "data/cards/111.png",
        "data/cards/112.png",
        "data/cards/113.png",
        "data/cards/114.png",
        "data/cards/115.png",
        "data/cards/116.png",
        "data/cards/11.png",
        "data/cards/120.png",
        "data/cards/121.png",
        "data/cards/125.png",
        "data/cards/126.png",
        "data/cards/127.png",
        "data/cards/12.png",
        "data/cards/130.png",
        "data/cards/131.png",
        "data/cards/132.png",
        "data/cards/135.png",
        "data/cards/136.png",
        "data/cards/13.png",
        "data/cards/140.png",
        "data/cards/141.png",
        "data/cards/145.png",
        "data/cards/146.png",
        "data/cards/14.png",
        "data/cards/150.png",
        "data/cards/151.png",
        "data/cards/152.png",
        "data/cards/15.png",
        "data/cards/160.png",
        "data/cards/16.png",
        "data/cards/170.png",
        "data/cards/175.png",
        "data/cards/17.png",
        "data/cards/18.png",
        "data/cards/19.png",
        "data/cards/1.png",
        "data/cards/2001.png",
        "data/cards/2002.png",
        "data/cards/2003.png",
        "data/cards/2004.png",
        "data/cards/2005.png",
        "data/cards/200.png",
        "data/cards/201.png",
        "data/cards/202.png",
        "data/cards/203.png",
        "data/cards/205.png",
        "data/cards/206.png",
        "data/cards/207.png",
        "data/cards/208.png",
        "data/cards/209.png",
        "data/cards/20.png",
        "data/cards/210.png",
        "data/cards/211.png",
        "data/cards/212.png",
        "data/cards/213.png",
        "data/cards/214.png",
        "data/cards/215.png",
        "data/cards/217.png",
        "data/cards/218.png",
        "data/cards/219.png",
        "data/cards/21.png",
        "data/cards/220.png",
        "data/cards/221.png",
        "data/cards/22.png",
        "data/cards/230.png",
        "data/cards/231.png",
        "data/cards/235.png",
        "data/cards/236.png",
        "data/cards/23.png",
        "data/cards/240.png",
        "data/cards/241.png",
        "data/cards/245.png",
        "data/cards/246.png",
        "data/cards/247.png",
        "data/cards/248.png",
        "data/cards/250.png",
        "data/cards/251.png",
        "data/cards/252.png",
        "data/cards/255.png",
        "data/cards/260.png",
        "data/cards/261.png",
        "data/cards/265.png",
        "data/cards/2.png",
        "data/cards/3001.png",
        "data/cards/3002.png",
        "data/cards/3003.png",
        "data/cards/3004.png",
        "data/cards/3005.png",
        "data/cards/300.png",
        "data/cards/301.png",
        "data/cards/302.png",
        "data/cards/303.png",
        "data/cards/305.png",
        "data/cards/306.png",
        "data/cards/307.png",
        "data/cards/308.png",
        "data/cards/309.png",
        "data/cards/310.png",
        "data/cards/311.png",
        "data/cards/312.png",
        "data/cards/313.png",
        "data/cards/320.png",
        "data/cards/321.png",
        "data/cards/322.png",
        "data/cards/325.png",
        "data/cards/326.png",
        "data/cards/330.png",
        "data/cards/331.png",
        "data/cards/332.png",
        "data/cards/335.png",
        "data/cards/336.png",
        "data/cards/337.png",
        "data/cards/340.png",
        "data/cards/341.png",
        "data/cards/342.png",
        "data/cards/343.png",
        "data/cards/344.png",
        "data/cards/350.png",
        "data/cards/351.png",
        "data/cards/352.png",
        "data/cards/355.png",
        "data/cards/360.png",
        "data/cards/365.png",
        "data/cards/366.png",
        "data/cards/367.png",
        "data/cards/368.png",
        "data/cards/3.png",
        "data/cards/4001.png",
        "data/cards/4002.png",
        "data/cards/4003.png",
        "data/cards/4004.png",
        "data/cards/4005.png",
        "data/cards/400.png",
        "data/cards/401.png",
        "data/cards/402.png",
        "data/cards/403.png",
        "data/cards/405.png",
        "data/cards/407.png",
        "data/cards/410.png",
        "data/cards/413.png",
        "data/cards/415.png",
        "data/cards/417.png",
        "data/cards/420.png",
        "data/cards/423.png",
        "data/cards/425.png",
        "data/cards/427.png",
        "data/cards/430.png",
        "data/cards/433.png",
        "data/cards/435.png",
        "data/cards/437.png",
        "data/cards/440.png",
        "data/cards/443.png",
        "data/cards/445.png",
        "data/cards/447.png",
        "data/cards/450.png",
        "data/cards/451.png",
        "data/cards/452.png",
        "data/cards/453.png",
        "data/cards/455.png",
        "data/cards/456.png",
        "data/cards/457.png",
        "data/cards/460.png",
        "data/cards/461.png",
        "data/cards/462.png",
        "data/cards/463.png",
        "data/cards/464.png",
        "data/cards/470.png",
        "data/cards/471.png",
        "data/cards/472.png",
        "data/cards/475.png",
        "data/cards/476.png",
        "data/cards/477.png",
        "data/cards/478.png",
        "data/cards/4.png",
        "data/cards/5001.png",
        "data/cards/5002.png",
        "data/cards/500.png",
        "data/cards/501.png",
        "data/cards/502.png",
        "data/cards/503.png",
        "data/cards/504.png",
        "data/cards/505.png",
        "data/cards/506.png",
        "data/cards/507.png",
        "data/cards/508.png",
        "data/cards/509.png",
        "data/cards/510.png",
        "data/cards/511.png",
        "data/cards/512.png",
        "data/cards/513.png",
        "data/cards/514.png",
        "data/cards/515.png",
        "data/cards/516.png",
        "data/cards/517.png",
        "data/cards/518.png",
        "data/cards/519.png",
        "data/cards/520.png",
        "data/cards/521.png",
        "data/cards/522.png",
        "data/cards/523.png",
        "data/cards/524.png",
        "data/cards/525.png",
        "data/cards/526.png",
        "data/cards/527.png",
        "data/cards/5.png",
        "data/cards/6.png",
        "data/cards/7.png",
        "data/cards/8.png",
        "data/cards/9.png",
    };

    static unordered_map<size_t, Card> cards;
    static optional<Font> font;
    static rangen rg;

    size_t idx = rg.nexti(filenames.size());

    if (auto it = cards.find(idx); it != cards.end())
    {
        return it->second;
    }

    char const* filename = filenames[idx];
    Texture2D tex = LoadTexture(filename);
    assert(IsTextureReady(tex));

    if (!font)
    {
        font = LoadFontEx("data/font/akademie_alte.ttf", 64, nullptr, 0);
        assert(IsFontReady(*font));
    }

    Card card{RANDOM.nextColor(), *font, tex};
    cards.insert({idx, card});

    return card;
}

cardlist_t Card::createTestCards(size_t n)
{
    cardlist_t cards;
    for (size_t i = 0; i < n; ++i)
        cards.push_back(createTestCard());
    return cards;
}





