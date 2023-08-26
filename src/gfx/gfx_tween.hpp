//
// Created by james on 8/24/23.
//

#include "gfx/gfx_easings.hpp"
#include "gfx/gfx_vlist.hpp"

#define M_DECL_PURE     [[nodiscard]] inline
#define M_DECL          inline

namespace gfx
{
    template <typename T> class TweenX
    {
    public:
        using func_type  = easings::func_t;
        using real_type  = easings::real_t;
        using vlist_type = typename VlistAdapter<T>::vlist_type;
        using value_type = typename VlistAdapter<T>::value_type;

        static constexpr real_type ZERO = real_type(0.0f);

        TweenX(func_type func, real_type time_end)
                : m_ease{func}, m_time_now{time_end}, m_time_end{time_end}
        {
            assert(func != nullptr);
            assert(time_end >= ZERO);
        }

        M_DECL_PURE real_type   progress() const { return m_time_now / m_time_end; }
        M_DECL_PURE bool        isAtSrc () const { return m_time_now == ZERO; }
        M_DECL_PURE bool        isAtDst () const { return m_time_now >= m_time_end; }
        M_DECL_PURE bool        isTween () const { return m_time_now > ZERO && m_time_now < m_time_end; }


        M_DECL_PURE value_type now() const { return value(m_vlist_now); }
        M_DECL_PURE value_type src() const { return value(m_vlist_src); }
        M_DECL_PURE value_type dst() const { return value(m_vlist_dst); }

        M_DECL void anim(value_type const& src, value_type const& dst)
        {
            m_time_now  = ZERO;
            m_vlist_now = vlist(src);
            m_vlist_src = vlist(src);
            m_vlist_dst = vlist(dst);
        }

        //
        // set
        //

        M_DECL void set(func_type func, real_type time_end)
        {
            assert(func != nullptr);
            assert(time_end >= ZERO);
            m_ease      = func;
            m_time_end  = time_end;
        }

        M_DECL void setToSrc()
        {
            m_time_now  = ZERO;
            m_vlist_now = m_vlist_src;
        }

        M_DECL void setToDst()
        {
            m_time_now  = m_time_end;
            m_vlist_now = m_vlist_dst;
        }

        M_DECL bool update(real_type time_delta)
        {
            assert(m_ease != nullptr);

            assert(m_time_end >= ZERO);
            assert(m_time_now >= ZERO);
            assert(m_time_now <= m_time_end);
            assert(time_delta >= ZERO);

            if (m_time_now >= m_time_end)
                return false;

            m_time_now += time_delta;
            if (m_time_now < m_time_end)
            {
                for(size_t i = 0; i < m_vlist_now.size(); ++i)
                    m_vlist_now[i] = m_ease(m_time_now, m_vlist_src[i], m_vlist_dst[i] - m_vlist_src[i], m_time_end);
            }
            else
            {
                m_time_now = m_time_end;
                for (size_t i = 0; i < m_vlist_now.size(); ++i)
                    m_vlist_now[i] = m_vlist_dst[i];
            }
            return true;
        }

    private:
        func_type m_ease      = nullptr;
        real_type m_time_now  = ZERO;
        real_type m_time_end  = ZERO;

        vlist_type m_vlist_src{};
        vlist_type m_vlist_dst{};
        vlist_type m_vlist_now{};

        M_DECL static value_type value(vlist_type const& v) { return VlistAdapter<T>::value(v); }
        M_DECL static vlist_type vlist(value_type const& v) { return VlistAdapter<T>::vlist(v); }
    };

    using TweenReal = TweenX<easings::real_t>;
    using TweenVec2 = TweenX<ut::vec2x<easings::real_t>>;
    using TweenVec3 = TweenX<ut::vec3x<easings::real_t>>;
    using TweenVec4 = TweenX<ut::vec4x<easings::real_t>>;
    using TweenRect = TweenX<ut::rectx<easings::real_t>>;
}

#undef M_DECL_PURE
#undef M_DECL