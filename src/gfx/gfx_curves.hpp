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


    template <size_t D>
    static ut::vecnf<D> cubicBezier(float t,
                                    ut::vecnf<D> const& p1,
                                    ut::vecnf<D> const& p2,
                                    ut::vecnf<D> const& p3,
                                    ut::vecnf<D> const& p4)
    {
        _flt a = powf((1.0f - t), 3.0f);
        _flt b = 3.0f * t * powf((1.0f - t), 2.0f);
        _flt c = 3.0f * powf(t, 2.0f) * (1.0f - t);
        _flt d = powf(t, 3.0f);

        return (p1 * a) + (p2 * b) + (p3 * c) + (p4 * d);
    }

}

#undef _vec
#undef _flt