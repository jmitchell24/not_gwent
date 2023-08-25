#pragma once

#include <ut/math.hpp>
#include <ut/color.hpp>

#include <cmath>
#include <vector>
#include <cassert>
#include <optional>

#define EASINGS_DECL    static inline
#define EASINGS_PI      3.14159265358979323846f

namespace gfx
{
namespace easings
{



    //
    //  t = current time (in any unit measure, but same unit as duration)
    //  b = starting value to interpolate
    //  c = the total change in value of b that needs to occur
    //  d = total time it should take to complete (duration)
    //

    using real_t = float;
    using func_t = real_t (*)(real_t t, real_t b, real_t c, real_t d);

    //
    // Linear Easing functions
    //

    EASINGS_DECL real_t linearNone   (real_t t, real_t b, real_t c, real_t d) { return (c*t/d + b); }
    EASINGS_DECL real_t linearIn     (real_t t, real_t b, real_t c, real_t d) { return (c*t/d + b); }
    EASINGS_DECL real_t linearOut    (real_t t, real_t b, real_t c, real_t d) { return (c*t/d + b); }
    EASINGS_DECL real_t linearInOut  (real_t t, real_t b, real_t c, real_t d) { return (c*t/d + b); }

    //
    // Sine Easing functions
    //

    EASINGS_DECL real_t sineIn       (real_t t, real_t b, real_t c, real_t d) { return (-c*cosf(t/d*(EASINGS_PI/2.0f)) + c + b); }
    EASINGS_DECL real_t sineOut      (real_t t, real_t b, real_t c, real_t d) { return (c*sinf(t/d*(EASINGS_PI/2.0f)) + b); }
    EASINGS_DECL real_t sineInOut    (real_t t, real_t b, real_t c, real_t d) { return (-c/2.0f*(cosf(EASINGS_PI*t/d) - 1.0f) + b); }

    //
    // Circular Easing functions
    //

    EASINGS_DECL real_t circIn       (real_t t, real_t b, real_t c, real_t d) { t /= d; return (-c*(sqrtf(1.0f - t*t) - 1.0f) + b); }
    EASINGS_DECL real_t circOut      (real_t t, real_t b, real_t c, real_t d) { t = t/d - 1.0f; return (c*sqrtf(1.0f - t*t) + b); }
    EASINGS_DECL real_t circInOut    (real_t t, real_t b, real_t c, real_t d)
    {
        if ((t/=d/2.0f) < 1.0f) return (-c/2.0f*(sqrtf(1.0f - t*t) - 1.0f) + b);
        t -= 2.0f; return (c/2.0f*(sqrtf(1.0f - t*t) + 1.0f) + b);
    }

    //
    // Cubic Easing functions
    //

    EASINGS_DECL real_t cubicIn      (real_t t, real_t b, real_t c, real_t d) { t /= d; return (c*t*t*t + b); }
    EASINGS_DECL real_t cubicOut     (real_t t, real_t b, real_t c, real_t d) { t = t/d - 1.0f; return (c*(t*t*t + 1.0f) + b); }
    EASINGS_DECL real_t cubicInOut   (real_t t, real_t b, real_t c, real_t d)
    {
        if ((t/=d/2.0f) < 1.0f) return (c/2.0f*t*t*t + b);
        t -= 2.0f; return (c/2.0f*(t*t*t + 2.0f) + b);
    }

    //
    // Quadratic Easing functions
    //

    EASINGS_DECL real_t quadIn       (real_t t, real_t b, real_t c, real_t d) { t /= d; return (c*t*t + b); }
    EASINGS_DECL real_t quadOut      (real_t t, real_t b, real_t c, real_t d) { t /= d; return (-c*t*(t - 2.0f) + b); }
    EASINGS_DECL real_t quadInOut    (real_t t, real_t b, real_t c, real_t d)
    {
        if ((t/=d/2) < 1) return (((c/2)*(t*t)) + b);
        return (-c/2.0f*(((t - 1.0f)*(t - 3.0f)) - 1.0f) + b);
    }

    //
    // Exponential Easing functions
    //

    EASINGS_DECL real_t expoIn       (real_t t, real_t b, real_t c, real_t d) { return (t == 0.0f) ? b : (c*powf(2.0f, 10.0f*(t/d - 1.0f)) + b); }
    EASINGS_DECL real_t expoOut      (real_t t, real_t b, real_t c, real_t d) { return (t == d) ? (b + c) : (c*(-powf(2.0f, -10.0f*t/d) + 1.0f) + b);    }
    EASINGS_DECL real_t expoInOut    (real_t t, real_t b, real_t c, real_t d)
    {
        if (t == 0.0f) return b;
        if (t == d) return (b + c);
        if ((t/=d/2.0f) < 1.0f) return (c/2.0f*powf(2.0f, 10.0f*(t - 1.0f)) + b);

        return (c/2.0f*(-powf(2.0f, -10.0f*(t - 1.0f)) + 2.0f) + b);
    }

    //
    // Back Easing functions
    //

    EASINGS_DECL real_t backIn(real_t t, real_t b, real_t c, real_t d)
    {
        float s = 1.70158f;
        float postFix = t/=d;
        return (c*(postFix)*t*((s + 1.0f)*t - s) + b);
    }

    EASINGS_DECL real_t backOut(real_t t, real_t b, real_t c, real_t d)
    {
        float s = 1.70158f;
        t = t/d - 1.0f;
        return (c*(t*t*((s + 1.0f)*t + s) + 1.0f) + b);
    }

    EASINGS_DECL real_t backInOut(real_t t, real_t b, real_t c, real_t d)
    {
        float s = 1.70158f;
        if ((t/=d/2.0f) < 1.0f)
        {
            s *= 1.525f;
            return (c/2.0f*(t*t*((s + 1.0f)*t - s)) + b);
        }

        float postFix = t-=2.0f;
        s *= 1.525f;
        return (c/2.0f*((postFix)*t*((s + 1.0f)*t + s) + 2.0f) + b);
    }

    //
    // Bounce Easing functions
    //

    EASINGS_DECL real_t bounceOut(real_t t, real_t b, real_t c, real_t d)
    {
        if ((t/=d) < (1.0f/2.75f))
        {
            return (c*(7.5625f*t*t) + b);
        }
        else if (t < (2.0f/2.75f))
        {
            float postFix = t-=(1.5f/2.75f);
            return (c*(7.5625f*(postFix)*t + 0.75f) + b);
        }
        else if (t < (2.5/2.75))
        {
            float postFix = t-=(2.25f/2.75f);
            return (c*(7.5625f*(postFix)*t + 0.9375f) + b);
        }
        else
        {
            float postFix = t-=(2.625f/2.75f);
            return (c*(7.5625f*(postFix)*t + 0.984375f) + b);
        }
    }

    EASINGS_DECL real_t bounceIn     (real_t t, real_t b, real_t c, real_t d) { return (c - bounceOut(d - t, 0.0f, c, d) + b); }
    EASINGS_DECL real_t bounceInOut  (real_t t, real_t b, real_t c, real_t d)
    {
        if (t < d/2.0f) return (bounceIn(t*2.0f, 0.0f, c, d)*0.5f + b);
        else return (bounceOut(t*2.0f - d, 0.0f, c, d)*0.5f + c*0.5f + b);
    }

    //
    // Elastic Easing functions
    //

    EASINGS_DECL real_t elasticIn(real_t t, real_t b, real_t c, real_t d)
    {
        if (t == 0.0f) return b;
        if ((t/=d) == 1.0f) return (b + c);

        float p = d*0.3f;
        float a = c;
        float s = p/4.0f;
        float postFix = a*powf(2.0f, 10.0f*(t-=1.0f));

        return (-(postFix*sinf((t*d-s)*(2.0f*EASINGS_PI)/p )) + b);
    }

    EASINGS_DECL real_t elasticOut(real_t t, real_t b, real_t c, real_t d)
    {
        if (t == 0.0f) return b;
        if ((t/=d) == 1.0f) return (b + c);

        float p = d*0.3f;
        float a = c;
        float s = p/4.0f;

        return (a*powf(2.0f,-10.0f*t)*sinf((t*d-s)*(2.0f*EASINGS_PI)/p) + c + b);
    }

    EASINGS_DECL real_t elasticInOut(real_t t, real_t b, real_t c, real_t d)
    {
        if (t == 0.0f) return b;
        if ((t/=d/2.0f) == 2.0f) return (b + c);

        float p = d*(0.3f*1.5f);
        float a = c;
        float s = p/4.0f;

        if (t < 1.0f)
        {
            float postFix = a*powf(2.0f, 10.0f*(t-=1.0f));
            return -0.5f*(postFix*sinf((t*d-s)*(2.0f*EASINGS_PI)/p)) + b;
        }

        float postFix = a*powf(2.0f, -10.0f*(t-=1.0f));

        return (postFix*sinf((t*d-s)*(2.0f*EASINGS_PI)/p)*0.5f + c + b);
    }

#undef EASINGS_PI
#undef EASINGS_DECL

}
}

//
// variant of Tween that uses absolute 'GetTime()' vs delta 'GetFrametime()' functions
//

//    struct Tween
//    {
//    public:
//        easings::func_t m_ease      = nullptr;

//        easings::real_t time_off  = 0.0f;
//        easings::real_t m_time_now  = 0.0f;
//        easings::real_t m_time_end  = 0.0f;

//        easings::real_t m_value_src = 0.0f;
//        easings::real_t m_value_dst = 0.0f;
//        easings::real_t m_value_now = 0.0f;

//        inline void reset(easings::real_t m_time_now)
//        {
//            time_off = m_time_now;
//            m_time_now = 0.0f;
//        }

//        inline easings::real_t time() const { return m_time_now; }
//        inline easings::real_t value() const { return m_value_now; }

//        inline bool update(easings::real_t m_time_now)
//        {
//            assert(m_ease != nullptr);
//            assert(m_time_end > 0.0f);
//            assert(m_time_now >= 0.0f);
//            assert(m_time_now > time_off);
//            assert(m_time_now < m_time_end);

//            if (m_time_now <= (time_off + m_time_end))
//            {
//                m_time_now = m_time_now - time_off;
//                m_value_now = m_ease(m_time_now, m_value_src, m_value_dst - m_value_src, m_time_end);
//                return true;
//            }
//            m_value_now = m_value_dst;
//            return false;
//        }
//    };
