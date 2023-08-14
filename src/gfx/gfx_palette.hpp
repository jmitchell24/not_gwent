#pragma once

#include "types.hpp"
#include "Bind.hpp"

namespace lofi
{
    struct Palette
    {
        ut::color color;
        ut::color light;
        ut::color extra_light;
        ut::color dark;
        ut::color extra_dark;

        constexpr Palette withA(ut::b8 a) const
        {
            return
            {
                color.withA         (a),
                light.withA         (a),
                extra_light.withA   (a),
                dark.withA          (a),
                extra_dark.withA    (a)
            };
        }


        static constexpr Palette fromColor(ut::b32 i)
        {
            auto hsv = ut::color::hsv(ut::color(i));

            Palette p;
            p.extra_light   = ut::color(hsv.withV(1.));
            p.light         = ut::color(hsv.withV(.9));
            p.color         = ut::color(hsv.withV(.7));
            p.dark          = ut::color(hsv.withV(.4));
            p.extra_dark    = ut::color(hsv.withV(.1));
            return p;
        }

//        BEGIN_BIND
//            bind(color)
//            bind(light)
//            bind(extra_light)
//            bind(dark)
//            bind(extra_dark)
//        END_BIND
    };
}
