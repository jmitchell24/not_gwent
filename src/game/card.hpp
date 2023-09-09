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

            inline bool hasRow    () const { return IsTextureReady(row); }
            inline bool hasAbility() const { return IsTextureReady(ability); }
            inline bool hasArtwork() const { return IsTextureReady(artwork); }

            static Assets fromNgCard(ng::Card const& ng);
        };

        struct Layout
        {
            static constexpr float ASPECT_RATIO     = 1.43f;
            static constexpr float ASPECT_RATIO_INV = 0.70f;

            float w=0.0f,h=0.0f,x=0.0f,y=0.0f,z=1.0f;

            inline bool isValid() const { return w>0.0f && h>0.0f; }

            static Layout fromWidth (float w) { return { w, w*ASPECT_RATIO, 0.0f, 0.0f, 0.0f }; }
            static Layout fromHeight(float h) { return { h*ASPECT_RATIO_INV, h, 0.0f, 0.0f, 0.0f }; }
        };

        static constexpr float ELEVATION_GRAB       = 1.15f;
        static constexpr float ELEVATION_PEEK       = 1.08f;
        static constexpr float ELEVATION_DROP       = 1.00f;

        Layout      layout;
        Assets      assets;
        ng::Card    ng;

//        Card();
//        Card(Card const&)=default;

        inline float        getElevation    () const { return layout.z; }
        inline ut::vec2     getSize         () const { return {layout.w,layout.h}; }
        inline ut::vec2     getPosition2    () const { return {layout.x, layout.y}; }
        inline ut::vec3     getPosition3    () const { return {layout.x,layout.y,layout.z}; }
        inline ut::psize    getPsize        () const { return { getSize(), getPosition2() }; }
        inline ut::rect     getRect         () const { return ut::rect(getPsize()); }

        inline void setElevation(float f)                               { layout.z=f; }
        inline void setSize     (float w, float h)                      { layout.w=w; layout.h=h; }
        inline void setPosition2(float x, float y)                      { layout.x=x; layout.y=y; }
        inline void setPosition3(float x, float y, float z)             { layout.x=x; layout.y=y; layout.z=z; }
        inline void setPsize    (float w, float h, float x, float y)    { layout.w=w; layout.h=h; layout.x=x; layout.y=y; }

        inline void setSize     (ut::vec2   const& v) { layout.w=v.x; layout.h=v.y; }
        inline void setPosition2(ut::vec2   const& v) { layout.x=v.x; layout.y=v.y; }
        inline void setPosition3(ut::vec3   const& v) { layout.x=v.x; layout.y=v.y; layout.z=v.z; }
        inline void setPsize    (ut::psize  const& v) { setPsize(v.width, v.height, v.x, v.y); }
        inline void setRect     (ut::rect   const& v) { return setPsize(v.psize()); }

        inline ut::rect  getDrawRect () const { return getRect().anchorCCtoCC_W(layout.w * layout.z); }
        inline ut::color getDrawColor() const { return assets.color; }

        void draw();
        void drawDebug();

        //RenderTexture2D drawTexture();
        //RenderTexture2D drawTexture(float width, float height);

        static Card         createTestCard();
        static cardlist_t   createTestCards(size_t n);
    };
}