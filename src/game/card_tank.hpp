#pragma once

//
// Created by james on 8/16/23.
//

#include "record.hpp"

#include "game/card.hpp"
#include "context.hpp"

namespace game
{
    using card_ref_list_t = std::vector<CardRef>;
    using card_ref_list_param = std::vector<CardRef> const&;

    class CardTank
    {
    public:
        static CardTank& instance();

        bool hasCard(CardID id) const;
        Card& getCard(CardID id);

        //
        // single-card container functions
        //
        Card& addCard(Card card, CardLayer layer = CARD_LAYER_BOARD);
        void removeCard(CardID id);
        void elevateCard(CardID id);
        void moveCard(CardID id, CardLayer layer);
        void clear();

        //
        // multi-card container functions
        //
        card_ref_list_t addCardMulti(cardlist_param cards, CardLayer layer = CARD_LAYER_BOARD);
        void removeCardMulti(card_ref_list_param refs);

        Card&       addTestCard(CardLayer layer = CARD_LAYER_BOARD);
        card_ref_list_t  addTestCards(size_t n, CardLayer layer = CARD_LAYER_BOARD);

        void update(update_param u);
        void draw(CardLayer layer = CARD_LAYER_BOARD);
        void drawDebug();
#if 0
        template <typename T> void forEachCard(T&& callback, CardLayer layer = CARD_LAYER_BOARD)
        {
            check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");
            for (auto&& it: m_layers[layer].cards)
                std::invoke(callback, it.ref());
        }

        template <typename T> bool ifAllCards(T&& callback, CardLayer layer = CARD_LAYER_BOARD)
        {
            check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");
            for (auto&& it : m_layers[layer].cards)
                if (!static_cast<bool>(std::invoke(callback, it.ref())))
                    return false;
            return true;
        }

        template <typename T> bool ifAnyCard(T&& callback, CardLayer layer = CARD_LAYER_BOARD)
        {
            check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");
            for (auto&& it : m_layers[layer].cards)
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    return true;
            return false;
        }

        template <typename T> bool ifNoCard(T&& callback, CardLayer layer = CARD_LAYER_BOARD)
        {
            check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");
            for (auto&& it : m_layers[layer].cards)
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    return false;
            return true;
        }

        template <typename T> size_t countIfCard(T&& callback, CardLayer layer = CARD_LAYER_BOARD)
        {
            check((size_t)layer < NUM_CARD_LAYERS, "bad layer value");
            size_t cnt=0;
            for (auto&& it: m_layers[layer].cards)
            {
                if (static_cast<bool>(std::invoke(callback, it.ref())))
                    ++cnt;
            }
            return cnt;
        }
#endif
    private:
        struct Index { size_t layer_idx, card_idx; };

        struct Layer
        {
            using cardlist_type = std::vector<Card>;

            cardlist_type cards;
            size_t        next_order_value  = 0;

            size_t  nextOrderValue();
            void    reorderCards();
            void    normalizeOrderValues();
        };

        static constexpr size_t NUM_CARD_LAYERS = 3;

        using id_type           = CardID::value_type;
        using idlist_type       = std::vector<id_type>;
        using idmap_type        = std::unordered_map<id_type, Index>;
        using layerlist_type    = std::array<Layer, NUM_CARD_LAYERS>;


        id_type         m_next_id_value = 1;

        layerlist_type  m_layers;
        idmap_type      m_id_map;
        idlist_type     m_old_ids;

        CardTank();

        Index           getIndex(CardID id);
        Card&           getCard(Index idx);
        id_type         nextIdValue();

        void            assignIdValues(size_t layer_idx);
    };

    static CardTank& TANK = CardTank::instance();
}



































