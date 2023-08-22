//
// Created by james on 8/21/23.
//

#pragma once

#include <ut/math.hpp>

namespace gfx
{
namespace curves
{
    using real_t = float;

    template <size_t D>
    ut::vec<real_t, D> Bezier_interpolate(real_t u, ut::vec<real_t, D> const& P0, ut::vec<real_t, D> const& P1, ut::vec<real_t, D> const& P2, ut::vec<real_t, D> const& P3)
    {
//    vec2 point = u*u*u*((-1) * P0 + 3 * P1 - 3 * P2 + P3);
//    point +=u*u*(3*P0 - 6 * P1+ 3 * P2);
//    point +=u*((-3) * P0 + 3 * P1);
//    point +=P0;

        auto point =
            ( ( (P0 * -1) + (P1 * 3) - (P2 * 3) + P3) * u * u * u ) +
            ( ( (P0 *  3) - (P1 * 6) + (P2 * 3) ) * u * u ) +
            ( ( (P0 * -3) + (P1 * 3)) * u ) +
            ( P0 );

        return point;
    }
}




}