#pragma once

//
// Created by james on 8/16/23.
//

#include "gfx/gfx_easings.hpp"

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
        static constexpr float ELEVATION_HI    = 1.25f;
        static constexpr float ELEVATION_LO    = 1.00f;

        static constexpr float ASPECT_RATIO     = 1.43f;
        static constexpr float ASPECT_RATIO_INV = 0.70f;


        Card();
        Card(ut::color const& tint, Font const& font, Texture2D const& texture_back);

        inline Texture2D const& texture() const { return m_texture; }

        inline ut::color tint       () const { return ut::colors::white.withNormalA(m_opacity); }
        inline ut::rect  rect       () const { return ut::rect(m_psize).anchorCCtoCC_W(m_psize.width * m_elevation); }
        inline ut::vec2  position   () const { return m_psize.pos; }
        inline ut::vec2  size       () const { return m_psize.size; }
        inline float     elevation  () const { return m_elevation; }
        inline float     opacity    () const { return m_opacity; }

        inline bool containsPoint(ut::vec2 const& p) const
        {
            return rect().contains(p);
        }

        //
        // target
        //

        void targetPosition (ut::vec2 const& p);
        void targetElevation(float f);
        void targetOpacity  (float f);

        //
        // set
        //

        void setPosition    (ut::vec2 const& p);
        void setElevation   (float f);
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

        static Card         createTestCard();
        static cardlist_t   createTestCards(size_t n);

    private:
        Font            m_font;
        Texture2D       m_texture;

        ut::color       m_tint;

        ut::psize       m_psize;
        float           m_elevation;
        float           m_opacity;

        float           m_fuzz_rotation;
        ut::vec2        m_fuzz_position;

        gfx::Tween2     m_tween_pos;
        gfx::Tween1     m_tween_elevation;
        gfx::Tween1     m_tween_opacity;

        gfx::Tween2     m_tween_fuzz_position;
        gfx::Tween1     m_tween_fuzz_rot;

#ifndef NDEBUG
        bool        m_is_layout_ready = false;
#endif
    };
}