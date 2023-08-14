//
// Created by james on 8/14/23.
//

#define STRONG_SCALAR_DECLS(type_, field_type_, field_)                                             \
    static_assert(std::is_scalar_v<field_type_>, "not a scalar...");                                \
    field_type_ field_;                                                                             \
    inline constexpr explicit type_(field_type_ value) : value{value} {}                            \
    inline constexpr explicit operator field_type_() const { return value; }                        \
    inline constexpr bool operator== (type_ const& x) const { return (field_) == x.field_; }        \
    inline constexpr bool operator!= (type_ const& x) const { return !(*this == x); }               \
    inline constexpr bool operator<  (type_ const& x) const { return (field_) < x.field_; }         \
    inline constexpr bool operator>  (type_ const& x) const { return *this < x; }                   \
    inline constexpr bool operator<= (type_ const& x) const { return !(*this < x); }                \
    inline constexpr bool operator>= (type_ const& x) const { return !(x < *this); }

#define RECORD_DECLS(x_, ...)                                                    \
    inline auto tie() const { return std::tie(__VA_ARGS__); }                       \
    inline bool operator== (x_ const& x) const { return tie() == x.tie(); }         \
    inline bool operator!= (x_ const& x) const { return !(*this == x); }            \
    inline bool operator<  (x_ const& x) const { return tie() < x.tie(); }          \
    inline bool operator>  (x_ const& x) const { return *this < x; }                \
    inline bool operator<= (x_ const& x) const { return !(*this < x); }             \
    inline bool operator>= (x_ const& x) const { return !(x < *this); }
