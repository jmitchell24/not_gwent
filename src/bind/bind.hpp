#pragma once

#include "conv.hpp"

#include <string>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

#define BEGIN_BIND(x_) \
    constexpr static auto BIND_LABEL_DEFAULT = #x_; \
    void bind(bind::Binder& binder, bind::Binder::lbl_param name = BIND_LABEL_DEFAULT) \
    { using ut::operator""_sv; if (binder.push(name, bind::Binder::toID(this))) {

#define END_BIND \
    binder.pop(); } }

#define BIND_RW(x_)             binder.rw(#x_##_sv, {}, x_);
#define BIND_RW_OPT(x_, opt_)   binder.rw(#x_, opt_, x_);
#define BIND_RO(x_)             binder.ro(#x_##_sv, x_);


#define ENUM_BIND_SCALARS \
    SCALAR(i8       , std::int8_t   ) \
    SCALAR(i16      , std::int16_t  ) \
    SCALAR(i32      , std::int32_t  ) \
    SCALAR(i64      , std::int64_t  ) \
    SCALAR(u8       , std::uint8_t  ) \
    SCALAR(u16      , std::uint16_t ) \
    SCALAR(u32      , std::uint32_t ) \
    SCALAR(u64      , std::uint64_t ) \
    SCALAR(f32      , float         ) \
    SCALAR(f64      , double        )

namespace bind
{
    struct opt_none { } constexpr static OPT_NONE;

#define SCALAR(n_, t_) struct opt_##n_ { t_ min=0, max=0, speed=1; };
    ENUM_BIND_SCALARS
#undef SCALAR

    class Binder
    {
    public:
        using lbl_param = ut::cstrview const&;
        using id_param  = size_t;

        constexpr static id_param NULL_ID = 0;

        template <typename T>
        inline static id_param toID(T* t) { return reinterpret_cast<id_param>(t); }

        virtual bool push(lbl_param lbl, id_param id = NULL_ID)=0;
        virtual void pop()=0;

        void rw (lbl_param lbl, opt_none, ut::color x);
        void rw (lbl_param lbl, opt_none, lbl_param x);
        void rw (lbl_param lbl, opt_none, bool x);

#define SCALAR(n_, t_) \
        void rw (lbl_param lbl, opt_##n_ const& opt, t_             & x); \
        void rw (lbl_param lbl, opt_##n_ const& opt, ut::vec2x  <t_>& x); \
        void rw (lbl_param lbl, opt_##n_ const& opt, ut::vec3x  <t_>& x); \
        void rw (lbl_param lbl, opt_##n_ const& opt, ut::vec4x  <t_>& x); \
        void rw (lbl_param lbl, opt_##n_ const& opt, ut::rectx  <t_>& x);
ENUM_BIND_SCALARS
#undef SCALAR

        void ro (lbl_param lbl, ut::color x);
        void ro (lbl_param lbl, lbl_param x);
        void ro (lbl_param lbl, bool x);

#define SCALAR(n_, t_) \
        void ro (lbl_param lbl, t_              const& x); \
        void ro (lbl_param lbl, ut::vec2x  <t_> const& x); \
        void ro (lbl_param lbl, ut::vec3x  <t_> const& x); \
        void ro (lbl_param lbl, ut::vec4x  <t_> const& x); \
        void ro (lbl_param lbl, ut::rectx  <t_> const& x);
ENUM_BIND_SCALARS
#undef SCALAR

        template <typename X>
        void ro (lbl_param lbl, X const& x)
        {
            const_cast<X&>(x).bind(*this, lbl);
        }

        template <typename X>
        void ro (lbl_param lbl, X const* x)
        {
            ro(lbl, const_cast<X*>(x));
        }

        template <typename X>
        void ro (lbl_param lbl, X* x)
        {
            if (x == nullptr)
                onRO1_nullptr(lbl);
            else
                (*x).bind(*this, lbl);
        }

    protected:
        virtual void onRW1_color    (lbl_param lbl, ut::color* x)=0;
        virtual void onRW1_bool     (lbl_param lbl, bool* x)=0;

#define SCALAR(n_, t_) \
        virtual void onRW1_##n_ (lbl_param lbl, opt_##n_ const& opt, t_* x)=0; \
        virtual void onRW2_##n_ (lbl_param lbl, opt_##n_ const& opt, t_* x)=0; \
        virtual void onRW3_##n_ (lbl_param lbl, opt_##n_ const& opt, t_* x)=0; \
        virtual void onRW4_##n_ (lbl_param lbl, opt_##n_ const& opt, t_* x)=0;
ENUM_BIND_SCALARS
#undef SCALAR

        virtual void onRO1_color    (lbl_param lbl, ut::color x)=0;
        virtual void onRO1_str      (lbl_param lbl, lbl_param x)=0;
        virtual void onRO1_bool     (lbl_param lbl, bool x)=0;
        virtual void onRO1_nullptr  (lbl_param lbl)=0;

#define SCALAR(n_, t_) \
        virtual void onRO1_##n_ (lbl_param lbl, t_ const* x)=0; \
        virtual void onRO2_##n_ (lbl_param lbl, t_ const* x)=0; \
        virtual void onRO3_##n_ (lbl_param lbl, t_ const* x)=0; \
        virtual void onRO4_##n_ (lbl_param lbl, t_ const* x)=0;
ENUM_BIND_SCALARS
#undef SCALAR
    };
}
