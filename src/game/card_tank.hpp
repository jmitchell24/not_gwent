#pragma once

//
// Created by james on 8/16/23.
//

#include "game/card_layout.hpp"

#define ENUM_GAME_CARD_TANK_TARGETS \
    CASE(cpu_hand           , CPU_HAND          ) \
    CASE(cpu_deck           , CPU_DECK          ) \
    CASE(cpu_yard           , CPU_YARD          ) \
    CASE(cpu_special_melee  , CPU_SPECIAL_MELEE ) \
    CASE(cpu_special_ranged , CPU_SPECIAL_RANGED) \
    CASE(cpu_special_siege  , CPU_SPECIAL_SIEGE ) \
    CASE(cpu_combat_melee   , CPU_COMBAT_MELEE  ) \
    CASE(cpu_combat_ranged  , CPU_COMBAT_RANGED ) \
    CASE(cpu_combat_siege   , CPU_COMBAT_SIEGE  ) \
    CASE(usr_hand           , USR_HAND          ) \
    CASE(usr_deck           , USR_DECK          ) \
    CASE(usr_yard           , USR_YARD          ) \
    CASE(usr_special_melee  , USR_SPECIAL_MELEE ) \
    CASE(usr_special_ranged , USR_SPECIAL_RANGED) \
    CASE(usr_special_siege  , USR_SPECIAL_SIEGE ) \
    CASE(usr_combat_melee   , USR_COMBAT_MELEE  ) \
    CASE(usr_combat_ranged  , USR_COMBAT_RANGED ) \
    CASE(usr_combat_siege   , USR_COMBAT_SIEGE  )

namespace game
{
    enum CardTankTarget
    {
#define CASE(x_, y_) TARGET_##y_,
ENUM_GAME_CARD_TANK_TARGETS
#undef CASE
        TARGET_ENUM_COUNT_
    };

    struct CardTankSlot
    {
        static constexpr float ELEVATION_GRAB       = 1.15f;
        static constexpr float ELEVATION_PEEK       = 1.08f;
        static constexpr float ELEVATION_DROP       = 1.00f;

        Card    card;
        size_t  order=0;

        gfx::TweenVec2  m_tween_nudge   {&gfx::easings::elasticOut , 1.0f};
        gfx::TweenReal  m_tween_drop    {&gfx::easings::bounceOut  , 0.5f};

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

    using slotlist_t    = std::vector<CardTankSlot>;
    using indexlist_t  = std::vector<size_t>;

    struct CardTankRow
    {
        indexlist_t indices;
        ssize_t  idx_hovered=-1;
        ssize_t  idx_ghosted=-1;
    };

    class CardTankSpot { };

    class CardTankStack { };

    class CardTank
    {
    public:
    //private:
        ut::rect m_bounds;

        size_t m_next_order_value=0;

        slotlist_t  m_slots;
        indexlist_t m_slots_open;
        indexlist_t m_slots_draw;


        CardTankSlot& slot(size_t idx)
        {
            assert(idx < m_slots.size());
            return m_slots[idx];
        }



        size_t addSlot(Card const& card)
        {
            if (!m_slots_open.empty())
            {
                size_t idx = m_slots_open.back();
                m_slots_open.pop_back();

                CardTankSlot& slot = m_slots[idx];
                slot.order = nextOrderValue();
                slot.card = card;

                m_slots_draw.push_back(idx);
                return idx;
            }

            CardTankSlot slot;
            slot.order = nextOrderValue();
            slot.card = card;

            size_t idx = m_slots.size();
            m_slots.push_back(slot);
            m_slots_draw.push_back(idx);
            return idx;
        }

        void removeSlot(size_t idx)
        {

        }

        size_t nextOrderValue()
        {
            return m_next_order_value++;
        }

        void refreshDrawIndices()
        {
            // Recreate list of indices using the slot order value for sorting.

            m_slots_draw.clear();
            for (size_t i = 0; i < m_slots.size(); ++i)
                m_slots_draw.push_back(i);

            std::sort(m_slots_draw.begin(), m_slots_draw.end(), [&](auto &&a, auto &&b)
            {
                return m_slots[a].order < m_slots[b].order;
            });

            // Normalize the slot order values, and reset m_next_order_value.
            // Reduces risk of overflow, but is probably unnecessary when size_t is 32 or more bits.

            for (size_t i = 0; i < m_slots_draw.size(); ++i)
                m_slots[m_slots_draw[i]].order = i;
            m_next_order_value = m_slots_draw.size();
        }
    };
}



































