//
// Created by james on 8/21/23.
//

#pragma once

#include <ut/math.hpp>

#include <vector>

#define _vec ut::vec2f const&
#define _flt float

namespace gfx
{
    class CatmullRomSpline
    {
    public:
        using cpoints_type = std::vector<ut::vec2f>;

        ut::vec2f point(float t) const;
        void layout(cpoints_type const& cpoints, float alpha, float tension);

    private:
        struct Segment
        {
            ut::vec2f   m_p[4]{};
            float       m_tmin{};
            float       m_tmax{};

            ut::vec2f point(_flt t) const;
            void layout(_vec p0, _vec p1, _vec p2, _vec p3,
                        _flt alpha, _flt tension, _flt tmin, _flt tmax);
        };

        using segments_type = std::vector<Segment>;
        segments_type m_segments;
    };


    class BezierCurve
    {
    public:

    private:
    };
}

#undef _vec
#undef _flt