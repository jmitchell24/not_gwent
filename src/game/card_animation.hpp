//
// Created by james on 9/14/23.
//

#pragma once

#include "assert.hpp"

#include "gfx/gfx_tween.hpp"
#include "gfx/gfx_spring.hpp"
#include "gfx/gfx_curves.hpp"

namespace game
{
    struct CardAnimationTween
    {
        gfx::TweenVec2  m_tween_nudge   {&gfx::easings::elasticOut , 1.0f};
        gfx::TweenReal  m_tween_drop    {&gfx::easings::bounceOut  , 0.5f};

        bool update() { }
        void draw() { }



        void animDrop()
        {
            m_tween_drop.set(&gfx::easings::bounceOut  , 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_DROP);
        }

        void animPeek()
        {
            m_tween_drop.set(&gfx::easings::expoOut, 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_PEEK);
        }

        void animGrab()
        {
            m_tween_drop.set(&gfx::easings::expoOut, 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_GRAB);
        }

        void animNudge(ut::vec2f const& p)
        {
            m_tween_nudge.set(&gfx::easings::expoOut , 1.0f);
            m_tween_nudge.anim(card.getPosition2(), p);
        }

        void animThrow(ut::vec2f const& src, ut::vec2f  const& dst)
        {

            card.setPosition2(src);
            m_tween_nudge.set(&gfx::easings::expoOut , 0.75f);
            m_tween_nudge.anim(src, dst);

            m_tween_drop.set(&gfx::easings::bounceOut , 1.5f);
            m_tween_drop.anim(1.3, ELEVATION_DROP);
        }
    };

    struct CardAnimationSpring
    {
        gfx::SpringVec3 spring;

        bool update() { }
        void draw() { }
    };

    struct CardAnimationCurve
    {
        bool update() { }
        void draw() { }
    };

    struct CardAnimation
    {
        enum Type
        {
            TYPE_TWEEN,
            TYPE_CURVE,
            TYPE_SPRING
        } type;

        union
        {
            CardAnimationTween  u_tween;
            CardAnimationSpring u_spring;
            CardAnimationCurve  u_curve;
        } value;

        inline bool isTween () const { return type == TYPE_TWEEN; }
        inline bool isCurve () const { return type == TYPE_CURVE; }
        inline bool isSpring() const { return type == TYPE_SPRING; }

        inline CardAnimationTween&  asTween () { return value.u_tween; }
        inline CardAnimationCurve&  asCurve () { return value.u_curve; }
        inline CardAnimationSpring& asSpring() { return value.u_spring; }

        void update()
        {
            switch (type)
            {
                case TYPE_TWEEN : value.u_tween.update(); break;
                case TYPE_CURVE : value.u_curve.update(); break;
                case TYPE_SPRING: value.u_spring.update(); break;
                default:assert_case(Type);
            }
        }

        void draw()
        {
            switch (type)
            {
                case TYPE_TWEEN : value.u_tween.draw(); break;
                case TYPE_CURVE : value.u_curve.draw(); break;
                case TYPE_SPRING: value.u_spring.draw(); break;
                default:assert_case(Type);
            }
        }
    };
}