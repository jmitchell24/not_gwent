//
// Created by james on 9/16/23.
//

#pragma once

#include <ut/math.hpp>


namespace game::layout
{
    struct CardLayout
    {
        static constexpr float ASPECT_RATIO         = 1.43f;
        static constexpr float ASPECT_RATIO_INV     = 1.0f / ASPECT_RATIO;

        static constexpr float ELEVATION_GRAB       = 1.15f;
        static constexpr float ELEVATION_PEEK       = 1.08f;
        static constexpr float ELEVATION_DROP       = 1.00f;

        float m_w=0.0f,m_h=0.0f,m_x=0.0f,m_y=0.0f,m_z=1.0f;

        inline bool isValid() const { return m_w>0.0f && m_h>0.0f; }

        inline float        getElevation    () const { return m_z; }
        inline ut::vec2     getSize         () const { return {m_w, m_h}; }
        inline ut::vec2     getPosition2    () const { return {m_x, m_y}; }
        inline ut::vec3     getPosition3    () const { return {m_x, m_y, m_z}; }
        inline ut::psize    getPsize        () const { return { getSize(), getPosition2() }; }
        inline ut::rect     getRect         () const { return ut::rect(getPsize()); }

        inline void setElevation(float f)                               { m_z=f; }
        inline void setSize     (float w, float h)                      { m_w=w; m_h=h; }
        inline void setPosition2(float x, float y)                      { m_x=x; m_y=y; }
        inline void setPosition3(float x, float y, float z)             { m_x=x; m_y=y; m_z=z; }
        inline void setPSize    (float w, float h, float x, float y)    { m_w=w; m_h=h; m_x=x; m_y=y; }

        inline void setSize     (ut::vec2   const& v) { m_w=v.x; m_h=v.y; }
        inline void setPosition2(ut::vec2   const& v) { m_x=v.x; m_y=v.y; }
        inline void setPosition3(ut::vec3   const& v) { m_x=v.x; m_y=v.y; m_z=v.z; }
        inline void setPSize    (ut::psize  const& v) { setPSize(v.width, v.height, v.x, v.y); }
        inline void setRect     (ut::rect   const& v) { return setPSize(v.psize()); }

        static CardLayout fromWidth (float w) { return { w, heightFromWidth(w), 0.0f, 0.0f, 0.0f }; }
        static CardLayout fromHeight(float h) { return { widthFromHeight(h), h, 0.0f, 0.0f, 0.0f }; }

        static float widthFromHeight(float h) { return h * ASPECT_RATIO_INV; }
        static float heightFromWidth(float w) { return w * ASPECT_RATIO; }
    };
}