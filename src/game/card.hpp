#pragma once

//
// Created by james on 8/16/23.
//

#include "gfx/gfx_tween.hpp"

//
// raylib
//
#include "raylib.h"

namespace game
{
    class Card;
    using cardlist_t = std::vector<Card>;

    class Card
    {
    public:
        //static constexpr float ASPECT_RATIO = 1.88f;
        static constexpr float ELEVATION_HI    = 1.20f;
        static constexpr float ELEVATION_MID   = 1.10f;
        static constexpr float ELEVATION_LO    = 1.00f;

        static constexpr float ASPECT_RATIO     = 1.43f;
        static constexpr float ASPECT_RATIO_INV = 0.70f;


        Card();
        Card(ut::color const& color, Font const& font, Texture2D const& texture_back);

        //inline Texture2D const& texture() const { return m_texture; }

        //inline ut::color color      () const { return m_color; }
        //inline ut::rect  rect       () const { return ut::rect(m_psize).anchorCCtoCC_W(m_psize.width * m_elevation); }
        inline ut::vec2  position   () const { return m_position; }
        inline ut::vec2  size       () const { return {m_width, m_height}; }
        //inline float     elevation  () const { return m_elevation; }
        //inline float     opacity    () const { return m_opacity; }

//        inline bool containsPoint(ut::vec2 const& p) const
//        {
//            return rect().contains(p);
//        }

        //
        // target
        //

        void targetPosition (ut::vec2f const& p);
        void targetOffset   (ut::vec2f const& p);
        void targetElevation(float f);
        void targetRotation (float f);
        void targetOpacity  (float f);

        //
        // set
        //

        void setPosition    (ut::vec2f const& p);
        void setOffset      (ut::vec2f const& p);
        void setElevation   (float f);
        void setRotation    (float f);
        void setOpacity     (float f);

        //
        // anim
        //

        void animRaise();
        void animLower();
        void animMove(ut::vec2 const& p);
        void animNudge(ut::vec2 const& p);


        void layout(ut::rect const& bounds);
        void layout(ut::vec2 const& size);
        void update();
        void draw();
        void drawDebug();

        RenderTexture2D drawTexture();
        RenderTexture2D drawTexture(float width, float height);

        static Card         createTestCard();
        static cardlist_t   createTestCards(size_t n);

    private:
        Font            m_font;
        ut::color       m_color;
        Texture2D       m_texture;

        float           m_width;
        float           m_height;
        float           m_opacity;
        float           m_elevation;
        float           m_rotation;

        ut::vec2f       m_position;
        ut::vec2f       m_offset;

        gfx::TweenVec2  m_tween_position;
        gfx::TweenVec2  m_tween_offset;
        gfx::TweenReal  m_tween_rotation;
        gfx::TweenReal  m_tween_elevation;
        gfx::TweenReal  m_tween_opacity;


        inline ut::rect drawBounds() const
        {
            ut::vec2f  wh = {m_width, m_height};
            ut::vec2f  xy = m_position + m_offset;
            ut::psizef p  = ut::psizef(wh, xy);

            return ut::rect(p).anchorCCtoCC_W(p.width * m_elevation);
        }

        inline ut::color drawColor() const
        {
            return m_color.withNormalA(m_opacity);
        }

#ifndef NDEBUG
        bool            m_is_layout_ready = false;
#endif
    };
}