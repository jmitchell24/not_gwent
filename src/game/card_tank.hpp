#pragma once

//
// Created by james on 8/16/23.
//

#include "record.hpp"

#include "game/card.hpp"

#include <functional>

namespace game
{
    using cardrefs_t = std::vector<CardRef>;
    using cardrefs_param = std::vector<CardRef> const&;

    class CardTank
    {
    public:
        static CardTank& instance();

        bool    hasCard     (CardID id) const;
        Card&   getCard     (CardID id);

        Card&   addCard     (Card card);
        void    removeCard  (CardID id);
        void    elevateCard (CardID id);

        Card&       addTestCard();
        cardrefs_t  addTestCards(size_t n);

        void update(float dt);
        void draw();
        void drawDebug();

        template <typename T> void forEachCard(T&& callback)
        {
            for (auto&& it: m_cards)
                std::invoke(callback, it.ref());
        }

        template <typename T> bool ifAllCards(T&& callback)
        {
            for (auto&& it : m_cards)
                if (!static_cast<bool>(std::invoke(callback, it.ref())))
                    return false;
            return true;
        }

        template <typename T> bool ifAnyCard(T&& callback)
        {
            for (auto&& it : m_cards)
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    return true;
            return false;
        }

        template <typename T> bool ifNoCard(T&& callback)
        {
            for (auto&& it : m_cards)
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    return false;
            return true;
        }

        template <typename T> size_t countIfCard(T&& callback)
        {
            size_t cnt=0;
            for (auto&& it: m_cards)
            {
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    ++cnt;
            }
            return cnt;
        }

    private:
        using cardlist_type = std::vector<Card>;
        using id_type       = CardID::value_type;
        using idlist_type   = std::vector<id_type>;
        using idmap_type    = std::unordered_map<id_type, size_t>;

        size_t          m_next_order_value  = 0;
        id_type         m_next_id_value = 1;

        cardlist_type   m_cards;
        idmap_type      m_map;
        idlist_type     m_old_ids;

        CardTank();

        size_t          getIndex(CardID id);
        bool            tryGetIndex(CardID id, size_t& idx);

        size_t          nextOrderValue();
        id_type         nextIdValue();
        void            rebuildMap();
        void            reorderCards();
    };

    static CardTank& TANK = CardTank::instance();
}



































