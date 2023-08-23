//
// Created by james on 8/23/23.
//

#include "gfx/gfx_curves.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

//
// std
//
#include <cfloat>
using namespace std;

vec2f CatmullRomSpline::point(float t) const
{
    assert(t >= 0 && t <= 1);
    auto i = size_t(t * (1.0f - FLT_EPSILON) * float(m_segments.size()));
    assert(i >= 0 && i < m_segments.size());

    return m_segments[i].point(t);
}

void CatmullRomSpline::layout(cpoints_type const& cpoints, float alpha, float tension)
{
    assert(cpoints.size() >= 4);
    assert(alpha >= 0 && alpha <= 1);
    assert(tension >= 0 && tension <= 1);

    size_t sz = cpoints.size() - 3;
    float td = 1.0f / float(sz);
    float t0 = 0.0f;

    for (size_t i = 0; i < sz; ++i)
    {
        float t1 = t0 + td;

        Segment s;
        s.layout(
                cpoints[i + 0],
                cpoints[i + 1],
                cpoints[i + 2],
                cpoints[i + 3],
                alpha, tension, t0, t1);
        m_segments.push_back(s);

        t0 = t1;
    }
}

vec2f CatmullRomSpline::Segment::point(float t) const
{
    t = (t - m_tmin) / (m_tmax - m_tmin);
    return
            ( m_p[0] * t * t * t ) +
            ( m_p[1] * t * t ) +
            ( m_p[2] * t ) +
            ( m_p[3] );
}

void CatmullRomSpline::Segment::layout(
        vec2f const& p0,
        vec2f const& p1,
        vec2f const& p2,
        vec2f const& p3,
        float alpha, float tension,
        float tmin, float tmax)
{
    assert(alpha    >= 0 && alpha   <= 1);
    assert(tension  >= 0 && alpha   <= 1);
    assert(tmin     >= 0 && tmin    <= 1);
    assert(tmax     >= 0 && tmax    <= 1);
    assert(tmin < tmax);

    //https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html

    auto t01 = pow(p0.distance(p1), alpha);
    auto t12 = pow(p1.distance(p2), alpha);
    auto t23 = pow(p2.distance(p3), alpha);

    auto m1 = ( p2 - p1 + ( ( (p1 - p0) / t01 - (p2 - p0) / (t01 + t12) ) * t12 ) ) * (1.0f - tension);
    auto m2 = ( p2 - p1 + ( ( (p3 - p2) / t23 - (p3 - p1) / (t12 + t23) ) * t12 ) ) * (1.0f - tension);

    m_p[0] = ( (p1 - p2) *  2.0f ) + m1 + m2;
    m_p[1] = ( (p1 - p2) * -3.0f ) - m1 - m1 - m2;
    m_p[2] = m1;
    m_p[3] = p1;

    m_tmin = tmin;
    m_tmax = tmax;
}