#pragma once

//
// Created by james on 8/16/23.
//

#include "gfx/gfx_tween.hpp"
//#include "ng/cards.hpp"

//
// ng
//
#include "ng/ng_card.hpp"

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
        struct Assets
        {
            Texture2D   artwork;
            Texture2D   row;
            Texture2D   ability;
            Font        font;
            ut::color   color;

            inline bool hasRow() const
            {
                return IsTextureReady(row);
            }

            inline bool hasAbility() const
            {
                return IsTextureReady(ability);
            }

            inline bool hasArtwork() const
            {
                return IsTextureReady(artwork);
            }
        };

        //static constexpr float ASPECT_RATIO = 1.88f;
        static constexpr float ELEVATION_GRAB       = 1.15f;
        static constexpr float ELEVATION_PEEK       = 1.08f;
        static constexpr float ELEVATION_DROP       = 1.00f;

        static constexpr float ASPECT_RATIO     = 1.43f;
        static constexpr float ASPECT_RATIO_INV = 0.70f;


        Card();
        Card(Card const&)=default;
        explicit Card(ng::Card const& card);


        void setCard(ng::Card const& card);
        //inline Texture2D const& texture() const { return m_texture; }

        inline ng::Card const& card() const { return m_card; }

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
        void targetElevation(float f);
        void targetRotation (float f);
        void targetOpacity  (float f);

        //
        // set
        //

        void setPosition    (ut::vec2f const& p);
        void setElevation   (float f);
        void setRotation    (float f);
        void setOpacity     (float f);

        //
        // anim
        //

        void animDrop();
        void animPeek();
        void animGrab();

        void animNudge(ut::vec2f const& p);
        void animThrow(ut::vec2f const& src, ut::vec2f const& dst);


        void layout(ut::rect const& bounds);
        void layout(ut::vec2 const& size);
        bool update();
        void draw();
        void drawDebug();

        RenderTexture2D drawTexture();
        RenderTexture2D drawTexture(float width, float height);

        static Card         createTestCard();
        static cardlist_t   createTestCards(size_t n);

        inline ut::rect drawBounds() const
        {
            ut::vec2f  wh = {m_width, m_height};
            ut::vec2f  xy = m_position;
            ut::psizef p  = ut::psizef(wh, xy);

            return ut::rect(p).anchorCCtoCC_W(p.width * m_elevation);
        }

        inline ut::color drawColor() const
        {
            return m_assets.color.withNormalA(m_opacity);
        }

    private:
        Assets          m_assets;
        ng::Card        m_card;

        float           m_width;
        float           m_height;
        float           m_opacity;
        float           m_elevation;
        float           m_rotation;

        ut::vec2f       m_position;

        gfx::TweenVec2  m_tween_nudge;
        gfx::TweenReal  m_tween_drop;

        gfx::TweenReal  m_tween_rotation;
        gfx::TweenReal  m_tween_opacity;


        static Assets assetsFromCard(ng::Card const& card);

#ifndef NDEBUG
        bool            m_is_layout_ready = false;
#endif
    };
}