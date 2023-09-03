//
// Created by james on 9/3/23.
//

#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_virt2d.hpp"
#include "game/assets.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

void Spinner::set(int value)
{
    if (m_value != value)
    {
        m_tween_target.setToDst();
        m_value = value;
    }
}

void Spinner::anim(int value)
{
    if (m_value != value)
    {
        m_tween_target.anim(float(m_value), float(value));
        m_value = value;
    }
}

void Spinner::layout(rect const& bounds)
{
    m_bounds = bounds;
    updateValues(float(m_value));
}

void Spinner::update(float dt)
{
    if (m_tween_target.update(dt))
        updateValues(m_tween_target.now());
}

void Spinner::draw()
{
    auto font = game::fonts::smallburgRegular128();
    VIRT.drawTextCCtoCC(font, m_bounds_a, PRINTER("%d", m_integer_a), m_color_a);
    VIRT.drawTextCCtoCC(font, m_bounds_b, PRINTER("%d", m_integer_b), m_color_b);

    VIRT_DEBUG(m_bounds);
    VIRT_DEBUG(m_bounds_a);
    VIRT_DEBUG(m_bounds_b);
}

void Spinner::updateValues(float f)
{
    int   i = int(f);
    float w = m_bounds.width();
    float n;

    if (f < 0.0f)
    {
        n = 1.0f - (float(i) - f);
        m_integer_a = i-1;
        m_integer_b = i;
    }
    else
    {
        n = f - float(i);
        m_integer_a = i;
        m_integer_b = i+1;
    }

    m_bounds_a = m_bounds.withOffsetX(-w*n);
    m_bounds_b = m_bounds_a.withOffsetX(w);

    n = easings::expoIn(n, 0, 1, 1);
    m_color_a  = colors::white.withNormalA(1.0f-n);
    m_color_b  = colors::white.withNormalA(n);
}