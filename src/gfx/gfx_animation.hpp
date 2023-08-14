#pragma once

#include <ut/math.hpp>
#include <ut/color.hpp>

#include <cmath>
#include <vector>
#include <cassert>

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

#define ENABLE_IF_1 template <size_t D_ = D, typename = std::enable_if_t<D_ == 1>>
#define ENABLE_IF_2 template <size_t D_ = D, typename = std::enable_if_t<D_ == 2>>
#define ENABLE_IF_3 template <size_t D_ = D, typename = std::enable_if_t<D_ == 3>>
#define ENABLE_IF_4 template <size_t D_ = D, typename = std::enable_if_t<D_ == 4>>

#define M_DECL_PURE             [[nodiscard]] inline
#define M_DECL                  inline

#define FOR_SIZE(i_)            for (size_t i_ = 0; i_ < D; ++i_)

    template <size_t D>
    struct TweenX
    {
        using values_type = easings::real_t[D];

        easings::func_t func      = nullptr;
        easings::real_t time_cur  = 0.0f;
        easings::real_t time_dur  = 0.0f;

        values_type value_src;
        values_type value_dst;
        values_type value_cur;

        M_DECL easings::real_t progress() const { return time_cur / time_dur; }
        M_DECL bool isAtSrc() const { return time_cur == 0.0f; }
        M_DECL bool isAtDst() const { return time_cur >= time_dur; }

        //
        // value convert
        //

#define DECL_GETTERs(p_, m_) \
        ENABLE_IF_1 M_DECL_PURE easings::real_t     p_##Scalar() const { return m_[0]; } \
        ENABLE_IF_2 M_DECL_PURE ut::vec2            p_##Vec2  () const { return ut::vec2 (m_); } \
        ENABLE_IF_3 M_DECL_PURE ut::vec3            p_##Vec3  () const { return ut::vec3 (m_); } \
        ENABLE_IF_4 M_DECL_PURE ut::vec4            p_##Vec4  () const { return ut::vec4 (m_); } \
        ENABLE_IF_4 M_DECL_PURE ut::rect            p_##Rect  () const { return ut::rect (m_[0], m_[1], m_[2], m_[3]); } \
        ENABLE_IF_4 M_DECL_PURE ut::color           p_##Color () const { return ut::color(m_[0], m_[1], m_[2], m_[3]); }

        DECL_GETTERs(cur, value_cur)
        DECL_GETTERs(src, value_src)
        DECL_GETTERs(dst, value_dst)
#undef DECL_GETTERs

        //
        // animTo
        //

        M_DECL void animToValues(values_type const& dst)
        {
            time_cur = 0.0f;

            FOR_SIZE(i)
            {
                value_src[i] = value_cur[i];
                value_dst[i] = dst[i];
            }
        }

        ENABLE_IF_1 M_DECL void animTo(easings::real_t  x) { animToValues({x});}
        ENABLE_IF_2 M_DECL void animTo(ut::vec2  const& v) { animToValues(v.pack); }
        ENABLE_IF_3 M_DECL void animTo(ut::vec3  const& v) { animToValues(v.pack); }
        ENABLE_IF_4 M_DECL void animTo(ut::vec4  const& v) { animToValues(v.pack); }
        ENABLE_IF_4 M_DECL void animTo(ut::rect  const& r) { animToValues({r.min.x, r.min.y, r.max.x, r.max.y}); }
        ENABLE_IF_4 M_DECL void animTo(ut::color const& c) { animToValues({c.r, c.g, c.b, c.a}); }

        //
        // animFuncTo
        //

        M_DECL void animFuncToValues(easings::func_t f, values_type const& dst)
        {
            assert(func != nullptr);

            func = f;
            time_cur = 0.0f;

            FOR_SIZE(i)
            {
                value_src[i] = value_cur[i];
                value_dst[i] = dst[i];
            }
        }

        ENABLE_IF_1 M_DECL void animFuncTo(easings::func_t f, easings::real_t  x) { animFuncToValues(f, {x});}
        ENABLE_IF_2 M_DECL void animFuncTo(easings::func_t f, ut::vec2  const& v) { animFuncToValues(f, v.pack); }
        ENABLE_IF_3 M_DECL void animFuncTo(easings::func_t f, ut::vec3  const& v) { animFuncToValues(f, v.pack); }
        ENABLE_IF_4 M_DECL void animFuncTo(easings::func_t f, ut::vec4  const& v) { animFuncToValues(f, v.pack); }
        ENABLE_IF_4 M_DECL void animFuncTo(easings::func_t f, ut::rect  const& r) { animFuncToValues(f, {r.min.x, r.min.y, r.max.x, r.max.y}); }
        ENABLE_IF_4 M_DECL void animFuncTo(easings::func_t f, ut::color const& c) { animFuncToValues(f, {c.r, c.g, c.b, c.a}); }

        //
        // animFromTo
        //

        M_DECL void animFromToValues(values_type const& src, values_type const& dst)
        {
            time_cur = 0.0f;

            FOR_SIZE(i)
            {
                value_src[i] = src[i];
                value_dst[i] = dst[i];
                value_cur[i] = src[i];
            }
        }

        ENABLE_IF_1 M_DECL void animFromTo(easings::real_t    src, easings::real_t    dst) { animFromToValues({src}, {dst});}
        ENABLE_IF_2 M_DECL void animFromTo(ut::vec2  const& src, ut::vec2  const& dst) { animFromToValues(src.pack, dst.pack); }
        ENABLE_IF_3 M_DECL void animFromTo(ut::vec3  const& src, ut::vec3  const& dst) { animFromToValues(src.pack, dst.pack); }
        ENABLE_IF_4 M_DECL void animFromTo(ut::vec4  const& src, ut::vec4  const& dst) { animFromToValues(src.pack, dst.pack); }

        ENABLE_IF_4 M_DECL void animFromTo(ut::rect  const& src, ut::rect  const& dst)
        { animFromToValues({src.min.x, src.min.y, src.max.x, src.max.y}, {dst.min.x, dst.min.y, dst.max.x, dst.max.y}); }

        ENABLE_IF_4 M_DECL void animFromTo(ut::color const& src, ut::color const& dst)
        { animFromToValues({src.r, src.g, src.b, src.a}, {dst.r, dst.g, dst.b, dst.a}); }

        //
        // set
        //

        M_DECL void setToSrc()
        {
            time_cur = 0.0f;
            FOR_SIZE(i) value_cur[i] = value_src[i];
        }

        M_DECL void setToDst()
        {
            time_cur = time_dur;
            FOR_SIZE(i) value_cur[i] = value_dst[i];
        }

        M_DECL void setToDstValues(values_type const& dst)
        {
            time_cur = time_dur;
            FOR_SIZE(i)
            {
                value_src[i] = dst[i];
                value_dst[i] = dst[i];
                value_cur[i] = dst[i];
            }
        }

        ENABLE_IF_1 M_DECL void setToDst(easings::real_t x)  { setToDstValues({x});}
        ENABLE_IF_2 M_DECL void setToDst(ut::vec2  const& v) { setToDstValues(v.pack); }
        ENABLE_IF_3 M_DECL void setToDst(ut::vec3  const& v) { setToDstValues(v.pack); }
        ENABLE_IF_4 M_DECL void setToDst(ut::vec4  const& v) { setToDstValues(v.pack); }
        ENABLE_IF_4 M_DECL void setToDst(ut::rect  const& r) { setToDstValues({r.min.x, r.min.y, r.max.x, r.max.y}); }
        ENABLE_IF_4 M_DECL void setToDst(ut::color const& c) { setToDstValues({c.r, c.g, c.b, c.a}); }

        //
        // update
        //

        M_DECL bool update(easings::real_t time_delta)
        {
            assert(func != nullptr);
            assert(time_dur     >   0.0f);
            assert(time_cur     >=  0.0f);
            assert(time_cur     <=  time_dur);
            assert(time_delta   >=  0.0f);

            if (time_cur >= time_dur)
                return false;

            time_cur += time_delta;
            if (time_cur < time_dur)
            {
                FOR_SIZE(i) value_cur[i] = func(time_cur, value_src[i], value_dst[i] - value_src[i], time_dur);
            }
            else
            {
                time_cur = time_dur;
                FOR_SIZE(i) value_cur[i] = value_dst[i];
            }
            return true;
        }

        //
        // factories
        //

        M_DECL static TweenX<D> makeValues(easings::func_t func, easings::real_t duration, values_type const& dst)
        {
            assert(func != nullptr);
            assert(duration > 0.0f);
            auto tmp = TweenX<D>{ func, duration, duration };
            FOR_SIZE(i)
            {
                tmp.value_src[i] = dst[i];
                tmp.value_dst[i] = dst[i];
                tmp.value_cur[i] = dst[i];
            }
            return tmp;
        }

        ENABLE_IF_1 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, easings::real_t dst)
        { return makeValues(func, duration, {dst}); }

        ENABLE_IF_2 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, ut::vec2 const& dst)
        { return makeValues(func, duration, dst.pack); }

        ENABLE_IF_3 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, ut::vec3 const& dst)
        { return makeValues(func, duration, dst.pack); }

        ENABLE_IF_4 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, ut::vec4 const& dst)
        { return makeValues(func, duration, dst.pack); }

        ENABLE_IF_4 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, ut::rect const& dst)
        { return makeValues(func, duration, {dst.min.x, dst.min.y, dst.max.x, dst.max.y}); }

        ENABLE_IF_4 M_DECL static TweenX<D> make(easings::func_t func, easings::real_t duration, ut::color const& dst)
        { return makeValues(func, duration, {dst.r, dst.g, dst.b, dst.a}); }
    };

#undef ENABLE_IF_1
#undef ENABLE_IF_2
#undef ENABLE_IF_3
#undef ENABLE_IF_4
#undef M_DECL_PURE
#undef M_DECL
#undef FOR_SIZE

    using Tween1 = TweenX<1>;
    using Tween2 = TweenX<2>;
    using Tween3 = TweenX<3>;
    using Tween4 = TweenX<4>;
}

//
// variant of Tween that uses absolute 'GetTime()' vs delta 'GetFrametime()' functions
//

//    struct Tween
//    {
//    public:
//        easings::func_t func      = nullptr;

//        easings::real_t time_off  = 0.0f;
//        easings::real_t time_cur  = 0.0f;
//        easings::real_t time_dur  = 0.0f;

//        easings::real_t value_src = 0.0f;
//        easings::real_t value_dst = 0.0f;
//        easings::real_t value_cur = 0.0f;

//        inline void reset(easings::real_t time_now)
//        {
//            time_off = time_now;
//            time_cur = 0.0f;
//        }

//        inline easings::real_t time() const { return time_cur; }
//        inline easings::real_t value() const { return value_cur; }

//        inline bool update(easings::real_t time_now)
//        {
//            assert(func != nullptr);
//            assert(time_dur > 0.0f);
//            assert(time_cur >= 0.0f);
//            assert(time_now > time_off);
//            assert(time_cur < time_dur);

//            if (time_now <= (time_off + time_dur))
//            {
//                time_cur = time_now - time_off;
//                value_cur = func(time_cur, value_src, value_dst - value_src, time_dur);
//                return true;
//            }
//            value_cur = value_dst;
//            return false;
//        }
//    };
