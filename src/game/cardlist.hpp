#pragma once

//
// Created by james on 8/16/23.
//

#include "game/card.hpp"

namespace game
{
    struct CardLayout
    {
        enum Direction { DIR_LEFT, DIR_CENTER };

        ut::rect    bounds;

        size_t      card_count;
        float       card_width;
        float       card_gap;

        static CardLayout create(Direction direction, ut::rect const& bounds, float card_width, size_t card_count);
        bool tryGetIndex(ut::vec2 const& mp, size_t& idx) const;

        [[nodiscard]] ut::vec2 getPos(size_t idx) const;
        [[nodiscard]] ut::rect getRect(size_t idx) const;
    };

    class CardList
    {
    public:
        CardList(CardLayout::Direction direction, cardlist_t cards);

        inline ut::rect const& bounds() const { return m_bounds; }

        inline size_t                   count       () const { return m_cards.size(); }
        inline CardLayout::Direction    direction   () const { return m_direction; }

        inline bool hasGhostedCard() const { return m_idx_ghosted >= 0; }
        inline bool hasHoveredCard() const { return m_idx_hovered >= 0; }

        inline ssize_t hoveredIndex() const { return m_idx_hovered; }
        inline ssize_t ghostedIndex() const { return m_idx_ghosted; }

        inline CardLayout const& layoutHovered() const { assert(m_is_layout_ready); return m_layout_hovered; }
        inline CardLayout const& layoutGhosted() const { assert(m_is_layout_ready); return m_layout_ghosted; }

        inline bool containsPoint(ut::vec2 const& mp) const { return m_bounds.contains(mp); }

        void layout(ut::rect const& bounds);
        void update();
        void draw();

        void setGhost(size_t idx);
        void clearGhost();

        void setHover(size_t idx);
        void clearHover();

        void addCard(size_t idx, Card const& card);
        Card removeCard(size_t idx);

    private:
        using indexlist_t = std::vector<size_t>;

        ssize_t         m_idx_hovered = -1;
        ssize_t         m_idx_ghosted = -1;

        CardLayout        m_layout_hovered;
        CardLayout        m_layout_ghosted;

        ut::rect        m_bounds;
        float           m_card_width;
        float           m_card_height;

        CardLayout::Direction      m_direction;
        cardlist_t      m_cards;
        indexlist_t     m_draw_indices;

        bool hasDrawIdx(size_t idx);
        void addDrawIdx(size_t idx);
        void removeDrawIdx(size_t idx);
        void replaceDrawIdx(size_t idx);
        void rebuildDrawIndices();

        void updateCardPositions(ssize_t place_idx = -1);
        void hover(ssize_t idx);

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}