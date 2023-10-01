#pragma once

#include "assert.hpp"

#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>

#include <raylib.h>


inline Color        torl(ut::color         const& c) { return {c.r, c.g, c.b, c.a }; }
inline Color        torl(ut::color::normal const& c) { return torl(c.toColor()); }
inline Color        torl(ut::color::hsluv  const& c) { return torl(c.toColor()); }
inline Color        torl(ut::color::hsv    const& c) { return torl(c.toColor()); }

inline Vector2      torl(ut::vec2i const& v) { return { (float)v.x, (float)v.y }; }
inline Vector2      torl(ut::vec2f const& v) { return { v.x, v.y }; }

inline Vector3      torl(ut::vec3i const& v) { return { (float)v.x, (float)v.y, (float)v.z }; }
inline Vector3      torl(ut::vec3f const& v) { return { v.x, v.y, v.z }; }

inline Vector4      torl(ut::vec4i const& v) { return { (float)v.x, (float)v.y, (float)v.z, (float)v.w }; }
inline Vector4      torl(ut::vec4f const& v) { return { v.x, v.y, v.z, v.w }; }

inline Rectangle    torl(ut::psizef const& p) { return { (float)p.x, (float)p.y, (float)p.width, (float)p.height }; }
inline Rectangle    torl(ut::psizei const& p) { return { (float)p.x, (float)p.y, (float)p.width, (float)p.height }; }

inline Rectangle    torl(ut::rectf const& r) { return torl(r.psize()); }
inline Rectangle    torl(ut::recti const& r) { return torl(r.psize()); }


inline ut::color tout(Color     const& c) { return { c.r, c.g, c.b, c.a }; }
inline ut::vec2  tout(Vector2   const& v) { return { v.x, v.y }; }
inline ut::vec3  tout(Vector3   const& v) { return { v.x, v.y, v.z }; }
inline ut::vec4  tout(Vector4   const& v) { return { v.x, v.y, v.z, v.w }; }
inline ut::rectf tout(Rectangle const& r) { return ut::rectf( ut::psizef(r.width, r.height, r.x, r.y) ); }
