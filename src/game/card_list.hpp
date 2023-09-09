#pragma once

//
// Created by james on 8/16/23.
//

#include "game/card_container.hpp"

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

    struct CardListSlot
    {
        static constexpr float ELEVATION_GRAB       = 1.15f;
        static constexpr float ELEVATION_PEEK       = 1.08f;
        static constexpr float ELEVATION_DROP       = 1.00f;

        Card    card;
        size_t  order=0;

        gfx::TweenVec2  m_tween_nudge{&gfx::easings::elasticOut , 1.0f};
        gfx::TweenReal  m_tween_drop{&gfx::easings::bounceOut  , 0.5f};

        void animDrop()
        {
            m_tween_drop.set(&gfx::easings::bounceOut  , 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_DROP);
        }

        void animPeek()
        {
            m_tween_drop.set(&gfx::easings::expoOut, 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_PEEK);
        }

        void animGrab()
        {
            m_tween_drop.set(&gfx::easings::expoOut, 0.5f);
            m_tween_drop.anim(card.getElevation(), ELEVATION_GRAB);
        }

        void animNudge(ut::vec2f const& p)
        {
            m_tween_nudge.set(&gfx::easings::expoOut , 1.0f);
            m_tween_nudge.anim(card.getPosition2(), p);
        }

        void animThrow(ut::vec2f const& src, ut::vec2f  const& dst)
        {

            card.setPosition2(src);
            m_tween_nudge.set(&gfx::easings::expoOut , 0.75f);
            m_tween_nudge.anim(src, dst);

            m_tween_drop.set(&gfx::easings::bounceOut , 1.5f);
            m_tween_drop.anim(1.3, ELEVATION_DROP);
        }

        bool update()
        {
            auto dt = GetFrameTime();
            auto b = false;

            if (m_tween_nudge.update(dt)) { b = true; card.setPosition2(m_tween_nudge.now()); }
            if (m_tween_drop     .update(dt)) { b = true; card.setElevation(m_tween_drop.now()); }

            return b;
        }

        void draw()
        {
            card.draw();
        }
    };

    class CardList : public CardContainer
    {
    public:
        CardList(CardLayout::Direction direction, cardlist_t cards);

        inline ut::rect const& bounds() const { return m_bounds; }

        inline bool                     empty       () const { return m_slots.empty(); }
        inline size_t                   count       () const { return m_slots.size(); }
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

        //
        // CardContainer -> Overrides
        //


        void setGhost(size_t idx) override;
        void clearGhost() override;
        inline bool tryGetGhostIndex(ut::vec2 const& mp, size_t& idx) override
        { return layoutGhosted().tryGetIndex(mp, idx); }
        inline bool hasGhost() const override
        { return m_idx_ghosted >= 0; }
        inline ut::rect getGhostRect() const override
        { assert(hasGhost()); return layoutGhosted().getRect(m_idx_ghosted); }

        void setHover(size_t idx) override;
        void clearHover() override;
        inline bool tryGetHoveredIndex(ut::vec2 const& mp, size_t& idx) override
        { return layoutHovered().tryGetIndex(mp, idx); }
        inline bool hasHover() const override
        { return m_idx_hovered >= 0; }
        inline ut::rect getHoverRect() const override
        { assert(hasHover()); return layoutHovered().getRect(m_idx_hovered); }

        void dropCard(size_t idx, Card const& card) override;
        void throwCard(size_t idx, Card const& card) override;
        Card removeCard(size_t idx) override;



    private:
        using slotlist_t = std::vector<CardListSlot>;
        using indexlist_t = std::vector<size_t>;

        size_t          m_next_order_value      = 0;
        ssize_t         m_idx_hovered           = -1;
        ssize_t         m_idx_ghosted           = -1;

        CardLayout        m_layout_hovered;
        CardLayout        m_layout_ghosted;

        ut::rect        m_bounds;
        float           m_card_width;
        float           m_card_height;

        CardLayout::Direction      m_direction;

        slotlist_t      m_slots;
        indexlist_t     m_draw_indices;

        void refreshDrawIndices();
        void refreshCardPositions(ssize_t place_idx = -1);

        void hover(ssize_t idx);
        CardListSlot& addCard(size_t idx, Card const& card);

        size_t nextOrderValue();

#ifndef NDEBUG
        bool m_is_layout_ready = false;
#endif
    };
}