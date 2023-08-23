//
// Created by james on 8/21/23.
//

#pragma once

#include <ut/math.hpp>

#include <vector>

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

            ut::vec2f point(float t) const;
            void layout(ut::vec2f const& p0,
                        ut::vec2f const& p1,
                        ut::vec2f const& p2,
                        ut::vec2f const& p3,
                        float alpha, float tension,
                        float tmin, float tmax);
        };

        using segments_type = std::vector<Segment>;
        segments_type m_segments;
    };

}