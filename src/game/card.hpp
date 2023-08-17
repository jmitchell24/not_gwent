#pragma once

//
// Created by james on 8/16/23.
//

#include "gfx/gfx_animation.hpp"

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
        static constexpr float ASPECT_RATIO = 1.88f;

        Card();
        Card(Texture2D const& texture_back);

        inline Texture2D const& texture() const { return m_texture; }

        inline ut::color tint       () const { return ut::colors::white.withNormalA(m_opacity); }
        inline ut::rect  rect       () const { return ut::rect(m_psize).anchorCCtoCC_W(m_psize.width * (m_elevation/4 + 1)); }
        inline ut::vec2  position   () const { return m_psize.pos; }
        inline ut::vec2  size       () const { return m_psize.size; }
        inline float     elevation  () const { return m_elevation; }
        inline float     opacity    () const { return m_opacity; }

        inline bool containsPoint(ut::vec2 const& p) const
        {
            return rect().contains(p);
        }

        void targetPosition (ut::vec2 const& p);
        void targetElevation(float f);
        void targetOpacity  (float f);

        void setPosition    (ut::vec2 const& p);
        void setElevation   (float f);
        void setOpacity     (float f);

        void layout(ut::vec2 const& size);
        void update();
        void draw();

        static Card         createTestCard();
        static cardlist_t   createTestCards(size_t n);

    private:
        Texture2D       m_texture;

        ut::psize       m_psize;
        float           m_elevation;
        float           m_opacity;

        gfx::Tween2     m_tween_pos;
        gfx::Tween1     m_tween_elevation;
        gfx::Tween1     m_tween_opacity;

#ifndef NDEBUG
        ut::color   m_outline_color;
        bool        m_is_layout_ready = false;
#endif
    };
}