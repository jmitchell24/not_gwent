//
// Created by james on 8/14/23.
//

#include <tuple>
#include <functional>

#define STRONG_SCALAR_DECLS(type_, field_type_, field_)                                             \
    static_assert(std::is_scalar_v<field_type_>, "not a scalar...");                                \
    field_type_ field_;                                                                             \
    inline constexpr explicit type_(field_type_ x) : value{x} {}                                    \
    inline constexpr explicit operator field_type_() const { return value; }                        \
    inline constexpr bool operator== (type_ const& x) const { return (field_) == x.field_; }        \
    inline constexpr bool operator!= (type_ const& x) const { return !(*this == x); }               \
    inline constexpr bool operator<  (type_ const& x) const { return (field_) < x.field_; }         \
    inline constexpr bool operator>  (type_ const& x) const { return *this < x; }                   \
    inline constexpr bool operator<= (type_ const& x) const { return !(*this < x); }                \
    inline constexpr bool operator>= (type_ const& x) const { return !(x < *this); }                \
    inline constexpr type_& operator++ () { ++field_; return *this; }                               \
    inline constexpr type_& operator-- () { --field_; return *this; }                               \
    inline constexpr type_ operator++ (int) { type_ t{*this}; return ++t; }                         \
    inline constexpr type_ operator-- (int) { type_ t{*this}; return --t; }


#define RECORD_DECLS(x_, ...) \
    inline auto tie() const { return std::tie(__VA_ARGS__); }                       \
    inline bool operator== (x_ const& x) const { return tie() == x.tie(); }         \
    inline bool operator!= (x_ const& x) const { return !(*this == x); }            \
    inline bool operator<  (x_ const& x) const { return tie() < x.tie(); }          \
    inline bool operator>  (x_ const& x) const { return *this < x; }                \
    inline bool operator<= (x_ const& x) const { return !(*this < x); }             \
    inline bool operator>= (x_ const& x) const { return !(x < *this); }
