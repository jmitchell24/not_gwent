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
        DIR_LEFT, DIR_CENTER, DIR_RIGHT
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
        static constexpr float ELEVATION_HOVERED        = 1.0f;
        static constexpr float ELEVATION_NOT_HOVERED    = 0.0f;

        static constexpr float OPACITY_GRABBED = 0.5f;
        static constexpr float OPACITY_NOT_GRABBED = 1.0f;



        CardList(Direction direction, cardlist_t cards);


        inline size_t       count       () const { return m_cards.size(); }
        inline Direction    direction   () const { return m_direction; }

        inline bool hasGhostedCard() const { return m_idx_ghosted >= 0; }
        inline bool hasHoveredCard() const { return m_idx_hovered >= 0; }



        inline bool containsPoint(ut::vec2 const& mp) const
        {
            return m_bounds.contains(mp);
        }

        bool hasGhost() const { return m_idx_ghosted >= 0; }

        Card cardToGhost(size_t idx)
        {
            assert(idx < m_cards.size());

            Card card = m_cards[idx];
            m_cards.erase(m_cards.begin()+idx);
            m_idx_ghosted = (ssize_t)idx;
            return card;
        }

        void ghostToCard(Card const& new_card)
        {
            assert(m_idx_ghosted >= 0);
            assert(m_idx_ghosted <= m_cards.size());

            size_t idx = m_idx_ghosted;

            Card& card = *m_cards.insert(m_cards.begin()+idx, new_card);
            card.layout({m_card_width, m_card_height});
            card.setPosition(calcCardPos(idx, m_cards.size()));

            card.setElevation(3.0f);
            card.targetElevation(0.0f);

            card.setOpacity(0.5f);
            card.targetOpacity(1.0f);

            m_ptoi_ghosted = PointToIndex::create(m_direction, m_card_width, m_bounds_pad, m_cards.size()+1);
            m_ptoi_hovered = PointToIndex::create(m_direction, m_card_width, m_bounds_pad, m_cards.size());

            m_idx_ghosted = -1;
            m_idx_hovered = -1;
        }

        void setGhost(size_t idx)
        {
            assert(idx <= m_cards.size());
            if (m_idx_ghosted != idx)
            {
                m_idx_ghosted = (ssize_t)idx;

                auto total_cards = m_cards.size() + 1;

                for (size_t i = 0; i < m_idx_ghosted; ++i)
                {
                    m_cards[i].targetPosition(calcCardPos(i, total_cards));
                }

                for (size_t i = m_idx_ghosted; i < m_cards.size(); ++i)
                {
                    m_cards[i].targetPosition(calcCardPos(i+1, total_cards));
                }
            }
        }

        void clearGhost()
        {
            if (m_idx_ghosted >= 0)
            {
                auto num_cards = m_cards.size();
                for (size_t i = 0; i < num_cards; ++i)
                    m_cards[i].targetPosition(calcCardPos(i, num_cards));
                m_idx_ghosted = -1;
            }
        }

        void setHover(size_t idx)
        {
            assert(idx < m_cards.size());
            hover(idx);
        }

        void clearHover()
        {
            hover(-1);
        }

        bool tryGetHoverIndex(ut::vec2 const& mp, ssize_t& idx) const;
        bool tryGetGhostIndex(ut::vec2 const& mp, ssize_t& idx) const;


        size_t  add     (size_t idx, Card const& card);
        bool    remove  (size_t idx);

        void layout(ut::rect const& bounds);
        void update();
        void draw();

    private:
        struct PointToIndex
        {
            ut::rect    bounds;
            size_t      count;

            static PointToIndex create(Direction direction, float card_width, ut::rect const& bounds, size_t card_count)
            {
                if (float w = card_width * float(card_count); w < bounds.width())
                {
                    switch (direction)
                    {
                        case DIR_LEFT  : return { bounds.anchorTLtoTL(w, bounds.height()), card_count };
                        case DIR_RIGHT : return { bounds.anchorTRtoTR(w, bounds.height()), card_count };
                        case DIR_CENTER: return { bounds.anchorCCtoCC(w, bounds.height()), card_count };

                        default: assert_case(Direction);
                    }

                    return { {}, 0 };
                }

                return { bounds, card_count };
            }

            bool tryGetIndex(ut::vec2 const& mp, ssize_t& idx) const
            {
                if (bounds.contains(mp))
                {
                    auto mp_min   = bounds.min.x;
                    auto mp_max   = bounds.max.x;
                    auto mp_width = mp_max - mp_min;

                    auto i = ssize_t( ((mp.x - mp_min) / (mp_width)) * float(count) );
                    idx = i;
                    return true;
                }
                return false;
            }
        };
        //enum CareRefreshType { CARD_REFRESH_MOVE, CARD_REFRESH_LAYOUT };

        ssize_t     m_idx_hovered = -1;
        ssize_t     m_idx_ghosted = -1;

        ut::rect    m_bounds;
        ut::rect    m_bounds_pad;
        float       m_card_width;
        float       m_card_height;

        PointToIndex m_ptoi_hovered;
        PointToIndex m_ptoi_ghosted;

        Direction   m_direction;
        cardlist_t  m_cards;

        //void refresh(CareRefreshType refresh);
        //void refreshAllCards(ut::rect const& origin, float increment, CareRefreshType refresh);

        inline bool isHoverValid() { return m_idx_hovered >= 0 && m_idx_hovered < m_cards.size(); }
        inline bool isGhostValid() { return m_idx_ghosted >= 0 && m_idx_ghosted <= m_cards.size(); }

        ut::vec2 calcCardPos(size_t idx, size_t num_cards) const;
        ut::rect calcCardRect(size_t idx, size_t num_cards) const
        {
            return ut::rect(ut::psize({m_card_width, m_card_height}, calcCardPos(idx, num_cards)));
        }



//        void layoutCard(size_t idx)
//        {
//            assert(m_is_layout_ready);
//            assert(idx < m_cards.size());
//
//
//            auto&& it = m_cards[idx];
//            it.layout({m_card_width, m_card_height});
//            it.setPosition(calcCardPos(idx));
//        }
//
//        void moveCard(size_t idx)
//        {
//            assert(m_is_layout_ready);
//            assert(idx < m_cards.size());
//
//            m_cards[idx].targetPosition(calcCardPos(idx));
//        }

        void moveAllCards()
        {
            assert(m_is_layout_ready);

            size_t num_cards = m_cards.size();
            for (size_t i = 0; i < num_cards; ++i)
            {
                m_cards[i].targetPosition(calcCardPos(i, num_cards));
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
