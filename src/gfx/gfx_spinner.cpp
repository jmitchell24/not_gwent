//
// Created by james on 9/3/23.
//

#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_virt2d.hpp"
#include "game/asset/assets.hpp"
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
    font = game::fonts::smallburgRegular128();
}

void Spinner::update(float dt)
{
    if (m_tween_target.update(dt))
        updateValues(m_tween_target.now());
}

void Spinner::draw()
{

    VIRT.drawTextCCtoCC(font, m_bounds_a, m_bounds_a.height(), PRINTER("%d", m_integer_a), m_color_a);
    VIRT.drawTextCCtoCC(font, m_bounds_b, m_bounds_b.height(), PRINTER("%d", m_integer_b), m_color_b);

    DRECT_PUSH2(Spinner, m_bounds);
    DRECT1(m_bounds_a);
    DRECT1(m_bounds_b);
    DRECT_POP();

}

void Spinner::updateValues(float f)
{
    switch (orientation)
    {

        case HORZ: updateValuesHorz(f); break;
        case VERT: updateValuesVert(f); break;
        default:nopath_case(Orientation);
    }
}

void Spinner::updateValuesHorz(float f)
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
    m_color_a  = color.withNormalA(1.0f-n);
    m_color_b  = color.withNormalA(n);
}

void Spinner::updateValuesVert(float f)
{
    int   i = int(f);
    float h = m_bounds.height();
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

    m_bounds_a = m_bounds.withOffsetY(-h*n);
    m_bounds_b = m_bounds_a.withOffsetY(h);

    n = easings::expoIn(n, 0, 1, 1);
    m_color_a  = color.withNormalA(1.0f-n);
    m_color_b  = color.withNormalA(n);
}