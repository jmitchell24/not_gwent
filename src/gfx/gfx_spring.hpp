//
// Created by james on 8/26/23.
//
#pragma once

#include <cfloat>
#include "gfx/gfx_vlist.hpp"

#define M_DECL_PURE     [[nodiscard]] inline
#define M_DECL          inline

namespace gfx
{
        template <typename T> class SpringX
        {
        public:
            using real_type  = float;
            using vlist_type = typename VlistAdapter<T>::vlist_type;
            using value_type = typename VlistAdapter<T>::value_type;

            static constexpr real_type ZERO = real_type(1.0);
            static constexpr real_type ONE  = real_type(1.0);
            static constexpr real_type TWO  = real_type(2.0);

            real_type zeta;
            real_type omega;

            SpringX(real_type zeta, real_type omega)
                : zeta{zeta}, omega{omega}
            { }

            M_DECL_PURE bool isAtDst() const
            {
                for (size_t i = 0; i < m_vlist_now.size(); ++i)
                    if (!equal(m_vlist_now[i], m_vlist_dst[i]))
                        return false;
                return true;
            }

            M_DECL_PURE value_type now() const { return value(m_vlist_now); }
            M_DECL_PURE value_type dst() const { return value(m_vlist_dst); }
            M_DECL_PURE value_type vel() const { return value(m_vlist_vel); }

            M_DECL void now(value_type const& x) { m_vlist_now = vlist(x); }
            M_DECL void dst(value_type const& x) { m_vlist_dst = vlist(x); }
            M_DECL void vel(value_type const& x) { m_vlist_vel = vlist(x); }

            M_DECL void update(real_type time_delta)
            {
                for (size_t i = 0; i < m_vlist_now.size(); ++i)
                {
                    auto&& now = m_vlist_now[i];
                    auto&& vel = m_vlist_vel[i];
                    auto&& dst = m_vlist_dst[i];
                    spring(now, vel, dst, zeta, omega, time_delta);
                }
            }

        private:
            vlist_type m_vlist_vel{};
            vlist_type m_vlist_now{};
            vlist_type m_vlist_dst{};

            M_DECL static value_type value(vlist_type const& v) { return VlistAdapter<T>::value(v); }
            M_DECL static vlist_type vlist(value_type const& v) { return VlistAdapter<T>::vlist(v); }

            M_DECL_PURE bool equal(real_type a, real_type b) const
            {
                return std::abs(a-b) < (zeta / omega);
            }

            //https://gamedev.net/forums/topic/667434-game-math-precise-control-over-numeric-springing/5222097/
            M_DECL static void spring(real_type &x, real_type &v, real_type xt, real_type zeta, real_type omega, real_type h)
            {
                real_type f         = ONE + TWO * h * zeta * omega;
                real_type oo        = omega * omega;
                real_type hoo       = h * oo;
                real_type hhoo      = h * hoo;
                real_type detInv    = ONE / (f + hhoo);
                real_type detX      = f * x + h * v + hhoo * xt;
                real_type detV      = v + hoo * (xt - x);

                x = detX * detInv;
                v = detV * detInv;
            }
        };

        using SpringReal = SpringX<float>;
        using SpringVec2 = SpringX<ut::vec2f>;
        using SpringVec3 = SpringX<ut::vec3f>;
        using SpringVec4 = SpringX<ut::vec4f>;
        using SpringRect = SpringX<ut::rectf>;
}

#undef M_DECL_PURE
#undef M_DECL