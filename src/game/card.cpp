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
#include <unordered_set>
using namespace std;

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "gfx/gfx_animation.hpp"
using namespace gfx;



//
// Card -> Implementation
//

Card::Card()
        : Card(createTestCard())
{}

Card::Card(Texture2D const& texture) :
        m_texture {texture}
{
    assert(IsTextureReady(texture));
}

void Card::setPosition(vec2 const& p)
{
    assert(m_is_layout_ready);

    m_psize.pos = p;
    m_tween_pos.setToDst(p);
}

void Card::setElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_elevation = f;
    m_tween_elevation.setToDst(f);
}

void Card::setOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);
    assert(f <= 1.0f);

    m_opacity = f;
    m_tween_opacity.setToDst(f);
}


void Card::targetPosition(vec2 const& p)
{
    assert(m_is_layout_ready);

    m_tween_pos.animTo(p);
}

void Card::targetElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_tween_elevation.animTo(f);
}

void Card::targetOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);
    assert(f <= 1.0f);

    m_tween_opacity.animTo(f);
}

void Card::layout(vec2 const& size)
{
    m_psize.size = size;

    m_elevation = 0.0f;
    m_opacity   = 1.0f;

    m_tween_pos         = Tween2::make(&easings::elasticOut , 1.0f, m_psize.pos);
    m_tween_elevation   = Tween1::make(&easings::bounceOut  , 0.5f, m_elevation);
    m_tween_opacity     = Tween1::make(&easings::expoOut    , 0.5f, m_opacity);

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void Card::update()
{
    assert(m_is_layout_ready);

    auto dt = GetFrameTime();
    if (m_tween_pos.update(dt))
    {
        m_psize.pos = m_tween_pos.curVec2();
    }

    if (m_tween_elevation.update(dt))
    {
        m_elevation = m_tween_elevation.curScalar();
    }

    if (m_tween_opacity.update(dt))
    {
        m_opacity = m_tween_opacity.curScalar();
    }
}

void Card::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(rect());

    auto r = rect();
    auto c = tint();

    VIRT.drawTexturePro(m_texture, r, c);


#ifndef NDEBUG

    auto outer = m_outline_color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.25f);

    VIRT.drawRectangle(r, inner.toColor());
    VIRT.drawRectangleLines(r, 2.0f, outer.toColor());
#endif
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
    static rangen rg;

    size_t idx = rg.nexti(filenames.size());

    if (auto it = cards.find(idx); it != cards.end())
    {
        return it->second;
    }

    char const* filename = filenames[idx];
    Texture2D tex = LoadTexture(filename);
    assert(IsTextureReady(tex));

    Card card{tex};
#ifndef NDEBUG
    card.m_outline_color = RANDOM.nextColor();
#endif

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





