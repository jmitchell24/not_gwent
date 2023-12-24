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
    struct CardID;
    struct CardRef;

    class Card;
    class CardTank;

    using cardlist_t = std::vector<Card>;
    using cardlist_param = cardlist_t const&;

    struct CardID
    {
        using value_type = std::uint64_t;
        value_type value=0;

        inline constexpr explicit operator value_type() const { return value; }

        inline constexpr bool isNil() const { return value == 0; }
        inline constexpr bool operator== (CardID const& x) const { return (value) == x.value; }
        inline constexpr bool operator!= (CardID const& x) const { return !(*this == x); }
        inline constexpr bool operator<  (CardID const& x) const { return (value) < x.value; }
        inline constexpr bool operator>  (CardID const& x) const { return *this < x; }
        inline constexpr bool operator<= (CardID const& x) const { return !(*this < x); }
        inline constexpr bool operator>= (CardID const& x) const { return !(x < *this); }
    };



    enum CardLayer
    {
        CARD_LAYER_BOARD,
        CARD_LAYER_OVERLAY
    };

    struct CardRef
    {
    public:
        CardID id{};

        bool inTank() const;

        inline bool isNil() const { return id.isNil(); }

        inline operator bool() const { return !id.isNil(); }
        inline bool operator== (CardRef const& x) const { return (id.value) == x.id.value; }
        inline bool operator!= (CardRef const& x) const { return !(*this == x); }
        inline bool operator<  (CardRef const& x) const { return (id.value) < x.id.value; }
        inline bool operator>  (CardRef const& x) const { return *this < x; }
        inline bool operator<= (CardRef const& x) const { return !(*this < x); }
        inline bool operator>= (CardRef const& x) const { return !(x < *this); }

        inline void reset() { id = {}; }

        Card& operator*();
        Card* operator->();

        Card const& operator*() const;
        Card const* operator->() const;
    };

    class Card
    {
        friend class CardTank;

    public:
        struct Assets
        {
            Texture2D   artwork {};
            Texture2D   card    {};
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

        CardLayout          layout;
        Assets              assets;

        gfx::SpringVec2     spring_pos          {0.25f, 6.0f * PI};
        gfx::SpringVec2     spring_size         {0.25f, 6.0f * PI};
        gfx::SpringReal     spring_elevation    {0.25f, 6.0f * PI};

        ng::Card            ng;

        inline bool isValid() const { return !m_id.isNil() && layout.isValid() && assets.isValid(); }
        inline bool isMoving() const { return m_is_animated; }
        inline bool isNotMoving() const { return !m_is_animated; }

        inline CardID       id   () const { return m_id; }
        inline CardRef      ref  () const { return CardRef{m_id}; }
        inline size_t       order() const { return m_order; }

        inline ut::rect  getDrawRect () const { return layout.getRect().anchorCCtoCC_W(layout.m_w * layout.m_z); }
        inline ut::color getDrawColor() const { return assets.color; }

        void animMove       (ut::vec2 const& p);
        void animResize     (ut::vec2 const& p);
        void animBounds     (ut::rect const& r);
        void animElevate    (float f);

        bool update(float dt);
        void draw();
        void drawDebug();

    private:
        CardID      m_id{};
        size_t      m_order=0;
        bool        m_is_animated=false;
    };


}