#pragma once

//
// Created by james on 8/16/23.
//

#include "game/card.hpp"

namespace game
{
    class CardList
    {
    public:
        enum AddAnim { ANIM_NONE, ANIM_CREATE, ANIM_MOVE };
        enum Direction { DIR_LEFT, DIR_CENTER };

        static constexpr float ELEVATION_HOVERED        = 1.0f;
        static constexpr float ELEVATION_NOT_HOVERED    = 0.0f;

        static constexpr float OPACITY_GRABBED = 0.5f;
        static constexpr float OPACITY_NOT_GRABBED = 1.0f;

        CardList(Direction direction, cardlist_t cards);

        inline size_t       count       () const { return m_cards.size(); }
        inline Direction    direction   () const { return m_direction; }

        inline bool hasGhostedCard() const { return m_idx_ghosted >= 0; }
        inline bool hasHoveredCard() const { return m_idx_hovered >= 0; }

        inline bool containsPoint(ut::vec2 const& mp) const { return m_bounds.contains(mp); }

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

            static CardCalc create(Direction direction, ut::rect const& bounds, float card_width, size_t card_count);
            bool tryGetIndex(ut::vec2 const& mp, size_t& idx) const;

            ut::vec2 getPos(size_t idx) const;
            ut::rect getRect(size_t idx) const;
        };

        ssize_t         m_idx_hovered = -1;
        ssize_t         m_idx_ghosted = -1;

        CardCalc        m_calc_hovered;
        CardCalc        m_calc_ghosted;

        ut::rect        m_bounds;
        float           m_card_width;
        float           m_card_height;

        Direction       m_direction;
        cardlist_t      m_cards;

        void updateCardPositions();
        void hover(ssize_t idx);

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}