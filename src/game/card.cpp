//
// Created by james on 8/16/23.
//

#include "random.hpp"

#include "game/card.hpp"
using namespace game;

//
// ut
//
using namespace ut;

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

    VIRT.drawTexture(m_texture, r, c);


#ifndef NDEBUG

    auto outer = m_outline_color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.75f);

    VIRT.drawRectangle(r, inner.toColor());
    VIRT.drawRectangleLines(r, 5.0f, outer.toColor());
#endif
}

Card Card::createTestCard()
{
    static Texture2D texture_decoy;

    if (!IsTextureReady(texture_decoy))
    {
        texture_decoy = LoadTexture("data/image/decoy.png");
        assert(IsTextureReady(texture_decoy));
    }

    Card card{texture_decoy};

#ifndef NDEBUG

    card.m_outline_color = RANDOM.nextColor();
#endif

    return card;
}

cardlist_t Card::createTestCards(size_t n)
{
    cardlist_t cards;
    for (size_t i = 0; i < n; ++i)
        cards.push_back(createTestCard());
    return cards;
}