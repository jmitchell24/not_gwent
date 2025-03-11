//
// Created by james on 9/20/23.
//

#pragma once

#include "game/card_tank.hpp"
#include "game/layout/row_layout.hpp"
#include "context.hpp"

namespace game
{
    using card_index_list_t = std::vector<size_t>;
    using card_index_list_param = std::vector<size_t> const&;

    class CardRefList
    {
    public:
        struct CardRefIndex { size_t idx; CardRef ref; };

        inline virtual ~CardRefList(){}

        inline auto data () const { return m_refs.data(); }
        inline auto begin() const { return m_refs.begin(); }
        inline auto end  () const { return m_refs.end(); }

        inline bool     empty   ()              const   { return m_refs.empty(); }
        inline size_t   count   ()              const   { return m_refs.size(); }
        inline bool     contains(CardRef ref)   const   { return getIdx(ref); }

        bool containsAny(card_ref_list_param refs) const;
        bool containsAll(card_ref_list_param refs) const;

        //
        // single-ref container functions
        //

        void clear();
        void push(cardref_param ref);
        CardRef pop(size_t idx);
        CardRef pop();
        void insert(size_t idx, cardref_param ref);
        void erase(size_t idx);
        void erase(cardref_param ref);
        CardRef get(size_t idx) const;


        //
        // multi-ref container functions
        //

        void insertMulti(size_t idx, card_ref_list_param refs);
        void eraseMulti(card_index_list_param indices);
        card_ref_list_t popMulti(card_index_list_param indices);
        card_ref_list_t getMulti(card_index_list_param indices) const;
        card_index_list_t getIndicesMulti(card_ref_list_param refs) const;

    protected:
        card_ref_list_t m_refs;

        virtual void onContainerChanged()
        { }

    private:
        ssize_t getIdx(cardref_param ref) const;
    };
}