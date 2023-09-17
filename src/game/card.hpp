#pragma once

//
// Created by james on 8/16/23.
//

#include "game/layout/card_layout.hpp"

//
// gfx
//
#include "gfx/gfx_spring.hpp"

//
// ng
//
#include "ng/ng_card.hpp"

//
// raylib
//
#include "raylib.h"

//
// ut
//
#include <ut/color.hpp>

namespace game
{
    class Card;
    class CardTank;

    using cardlist_t = std::vector<Card> ;

    class Card
    {
        friend class CardTank;

    public:
        struct ID
        {
            using value_type = std::uint64_t;

            value_type value=0;

            inline constexpr bool isValid() const { return value > 0; }

            inline constexpr explicit operator value_type() const { return value; }

            inline constexpr bool   operator== (ID const& x) const { return (value) == x.value; }
            inline constexpr bool   operator!= (ID const& x) const { return !(*this == x); }
            inline constexpr bool   operator<  (ID const& x) const { return (value) < x.value; }
            inline constexpr bool   operator>  (ID const& x) const { return *this < x; }
            inline constexpr bool   operator<= (ID const& x) const { return !(*this < x); }
            inline constexpr bool   operator>= (ID const& x) const { return !(x < *this); }

            inline constexpr ID&    operator++ () { ++value; return *this; }
            inline constexpr ID&    operator-- () { --value; return *this; }
            inline constexpr ID     operator++ (int) { auto t(*this); ++value; return t; }
            inline constexpr ID     operator-- (int) { auto t{*this}; --value; return t; }
        };

        struct Assets
        {
            Texture2D   artwork {};
            Texture2D   row     {};
            Texture2D   ability {};
            Font        font    {};
            ut::color   color   {};

            inline bool isValid() const { return hasArtwork(); }

            inline bool hasRow    () const { return IsTextureReady(row); }
            inline bool hasAbility() const { return IsTextureReady(ability); }
            inline bool hasArtwork() const { return IsTextureReady(artwork); }

            static Assets fromNgCard(ng::Card const& ng);
        };



        layout::CardLayout  layout;
        Assets              assets;
        gfx::SpringVec3     spring{0.25f, 6.0f * PI};

        ng::Card            ng;

        inline bool isValid() const { return m_id.isValid() && layout.isValid() && assets.isValid(); }
        inline ID id() const { return m_id; }
        inline size_t order() const { return m_order; }

        inline ut::rect  getDrawRect () const { return layout.getRect().anchorCCtoCC_W(layout.m_w * layout.m_z); }
        inline ut::color getDrawColor() const { return assets.color; }

        void draw();
        bool update(float dt);
        void drawDebug();

    private:
        ID      m_id{};
        size_t  m_order=0;
    };


}