//
// Created by james on 8/26/23.
//

#pragma once

#include <ut/math.hpp>

#define M_DECL          inline

namespace gfx
{
    template <typename> struct VlistAdapter { };

    template<> struct VlistAdapter<float>
    {
        using value_type = float;
        using vlist_type = std::array<float,1>;
        inline static value_type value(vlist_type const& v) { return v[0]; }
        inline static vlist_type vlist(value_type const& v) { return {v}; }
    };

    template<> struct VlistAdapter<ut::vec2f>
    {
        using value_type = ut::vec2f;
        using vlist_type = std::array<float,2>;
        M_DECL static value_type value(vlist_type const& v) { return {v[0],v[1]}; }
        M_DECL static vlist_type vlist(value_type const& v) { return {v.x, v.y}; }
    };

    template<> struct VlistAdapter<ut::vec3f>
    {
        using value_type = ut::vec3f;
        using vlist_type = std::array<float,3>;
        M_DECL static value_type value(vlist_type const& v) { return {v[0],v[1],v[2]}; }
        M_DECL static vlist_type vlist(value_type const& v) { return {v.x, v.y, v.z}; }
    };

    template<> struct VlistAdapter<ut::vec4f>
    {
        using value_type = ut::vec4f;
        using vlist_type = std::array<float,4>;
        M_DECL static value_type value(vlist_type const& v) { return {v[0],v[1],v[2],v[3]}; }
        M_DECL static vlist_type vlist(value_type const& v) { return {v.x, v.y, v.z, v.w}; }
    };

    template<> struct VlistAdapter<ut::rectf>
    {
        using value_type = ut::rectf;
        using vlist_type = std::array<float,4>;
        M_DECL static value_type value(vlist_type const& v) { return {v[0],v[1],v[2],v[3]}; }
        M_DECL static vlist_type vlist(value_type const& v) { return {v.min.x, v.min.y, v.max.x, v.max.y}; }
    };
}

#undef M_DECL