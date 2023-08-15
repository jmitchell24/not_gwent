#pragma once

#include "assert.hpp"
#include "record.hpp"

#include "game/object.hpp"
#include "gfx/gfx_animation.hpp"

//
// raylib
//
#include "raylib.h"

//
// ut
//

#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

//
// std
//
#include <vector>
#include <optional>


namespace game
{
    class Card;
    class CardList;
    class CardHold;

    using cardlist_t = std::vector<Card>;

    enum LayoutType
    {
        LAYOUT_LANDSCAPE,
        LAYOUT_PORTRAIT
    };

    enum Direction
    {
        DIR_LEFT, DIR_CENTER
    };

    enum PlayerType
    {
        PLAYER_HUMAN, PLAYER_CPU
    };

    enum Visibility
    {
        VIS_HIDDEN, VIS_REVEALED
    };

    enum CardElevation
    {

    };

    enum CardTarget
    {
        CARD_TARGET_NONE,
        CARD_TARGET_HAND,
        CARD_TARGET_MELEE,
        CARD_TARGET_RANGED,
        CARD_TARGET_SIEGE
    };

//    struct CardIndex
//    {
//        inline bool isValid() const { return value > 0; }
//
//        STRONG_SCALAR_DECLS(CardIndex, ssize_t, value)
//    };

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

    struct CardHold
    {
        inline bool isReady() const { return m_ready; }
        inline bool hasCard() const { return m_card.has_value(); }



        inline Card const& card() const { assert(m_card.has_value()); return *m_card; }
        inline void card(Card const& card) { m_card = card; }

        void init()
        {
            m_ready = true;
        }

        void update()
        {
            assert(isReady());
        }

        void draw()
        {
            assert(isReady());
        }

    private:
        bool m_ready = false;
        std::optional<Card> m_card;
    };

    class CardList
    {
    public:
        enum AddAnim { ANIM_NONE, ANIM_CREATE, ANIM_MOVE };

        static constexpr float ELEVATION_HOVERED        = 1.0f;
        static constexpr float ELEVATION_NOT_HOVERED    = 0.0f;

        static constexpr float OPACITY_GRABBED = 0.5f;
        static constexpr float OPACITY_NOT_GRABBED = 1.0f;

        CardList(Direction direction, cardlist_t cards);

        inline size_t       count       () const { return m_cards.size(); }
        inline Direction    direction   () const { return m_direction; }

        inline bool hasGhostedCard() const { return m_idx_ghosted >= 0; }
        inline bool hasHoveredCard() const { return m_idx_hovered >= 0; }

        inline bool containsPoint(ut::vec2 const& mp) const { return m_bounds_pad.contains(mp); }

        inline ssize_t hoveredIndex() const { return m_idx_hovered; }
        inline ssize_t ghostedIndex() const { return m_idx_ghosted; }

        bool tryGetHoverIndex(ut::vec2 const& mp, size_t& idx) const;
        bool tryGetGhostIndex(ut::vec2 const& mp, size_t& idx) const;

        void layout(ut::rect const& bounds);
        void update();
        void draw();

        void setGhost(size_t idx);
        void clearGhost();

        void setHover(size_t idx);
        void clearHover();

        void addCard(size_t idx, Card const& card, AddAnim anim = ANIM_CREATE);
        Card removeCard(size_t idx);

    private:
        struct CardCalc
        {
            ut::rect    bounds;

            size_t      card_count;
            float       card_width;
            float       card_gap;

            static CardCalc create(Direction direction, float card_width, ut::rect const& bounds, size_t card_count);
            bool tryGetIndex(ut::vec2 const& mp, size_t& idx) const;

            ut::vec2 getPos(size_t idx) const;
            ut::rect getRect(size_t idx) const;
        };

        ssize_t         m_idx_hovered = -1;
        ssize_t         m_idx_ghosted = -1;

        CardCalc        m_calc_hovered;
        CardCalc        m_calc_ghosted;

        ut::rect        m_bounds;
        ut::rect        m_bounds_pad;
        float           m_card_width;
        float           m_card_height;

        Direction       m_direction;
        cardlist_t      m_cards;

        void moveAllCards()
        {
            assert(m_is_layout_ready);

            for (size_t i = 0; i < m_cards.size(); ++i)
            {
                m_cards[i].targetPosition(m_calc_hovered.getPos(i));
            }
        }

        void hover(ssize_t idx)
        {
            assert(idx < (ssize_t)m_cards.size());
            if (m_idx_hovered != idx)
            {
                if (m_idx_hovered >= 0)
                    m_cards[m_idx_hovered].targetElevation(ELEVATION_NOT_HOVERED);

                m_idx_hovered = (ssize_t)idx;

                if (m_idx_hovered >= 0)
                    m_cards[m_idx_hovered].targetElevation(ELEVATION_HOVERED);
            }
        }

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };

    class CombatRow
    {
    public:
        CardList cards;

        inline CardTarget target() const { return m_target; }

        void layout(ut::rect const& bounds);
        void update();
        void draw();

        static CombatRow melee();
        static CombatRow ranged();
        static CombatRow siege();

    private:
        explicit CombatRow(CardTarget target);

        ut::rect m_bounds;
        CardTarget m_target;

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };



    class StatDisplay : public Layout
    {
    public:
        StatDisplay(PlayerType player_type, ut::cstrparam player_name);

        PlayerType playerType() const { return m_player_type; }
        ut::cstrview playerName() const { return m_player_name; }

        unsigned gems           () const { return 0; }
        unsigned score          () const { return 0; }
        unsigned librarySize    () const { return 0; }
        unsigned graveyardSize  () const { return 0; }

        void load();
        void unload();
        void update();
        void draw();

    protected:
        void onLayout(ut::rect const& bounds) override;

    private:
        std::array<ut::rect, 5> m_bounds_stats;

        PlayerType      m_player_type;
        ut::cstrview    m_player_name;

    };



    class PlayerBoard : public Layout
    {
    public:
        PlayerBoard(StatDisplay stats);

        void update();
        void draw();

    protected:
        void onLayout(ut::rect const& bounds) override;

    private:
        StatDisplay m_stats;
        CardList    m_cards;

        CombatRow   m_melee {CombatRow::melee()};
        CombatRow   m_ranged{CombatRow::ranged()};
        CombatRow   m_siege {CombatRow::siege()};
    };

    class Game : public Layout
    {
    public:

    protected:

    private:


        CardHold card_hold;
        PlayerBoard p1, p2;
//
//        void layout(rectf const& b)
//        {
//            bounds = b;
//
//            auto [p1_bounds, p2_bounds] = bounds.splitNV<2>(VIRT_PAD);
//            p1.init(ENEMY, p1_bounds, true);
//            p2.init(PLAYER, p2_bounds, false);
//        }
//
//        void update()
//        {
//            p1.update();
//            p2.update();
//        }
//
//        void draw()
//        {
//            VIRT_DEBUG(Game::bounds);
//
//            p1.draw();
//            p2.draw();
//        }
    };
}
