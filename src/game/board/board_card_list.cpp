//
// Created by james on 3/6/24.
//

#include "game/board/board_card_list.hpp"
using namespace game;

//
// ut
//

using namespace ut;

//
// std
//
using namespace std;

//
// Helper Function
//

inline static bool isNilAny(card_ref_list_param refs)
{
    for (auto&& it: refs)
        if (it.isNil())
            return true;
    return false;
}

inline static bool hasIdxAll(card_index_list_param indices, size_t sz)
{
    for (auto&& it: indices)
        if (!(it < sz))
            return false;
    return true;
}


bool CardRefList::containsAny(card_ref_list_param refs) const
{
    for (auto&& it: refs)
        if (contains(it))
            return true;
    return false;
}

bool CardRefList::containsAll(card_ref_list_param refs) const
{
    for (auto&& it: refs)
        if (!contains(it))
            return false;
    return true;
}

void CardRefList::clear()
{
    if (!m_refs.empty())
    {
        m_refs.clear();
        onContainerChanged();
    }
}

//
// implementation -> single-ref container functions
//

void CardRefList::push(cardref_param ref)
{
    check(!ref.isNil(), "ref is nil");
    check(!contains(ref), "card already in row");
    m_refs.push_back(ref);
    onContainerChanged();
}

CardRef CardRefList::pop(size_t idx)
{
    check(idx < m_refs.size(), "invalid card index");
    auto value = m_refs[idx];
    m_refs.erase(m_refs.begin()+(ssize_t)idx);
    onContainerChanged();
    return value;
}

CardRef CardRefList::pop()
{
    check(!m_refs.empty(), "list is empty");
    auto value = m_refs.back();
    m_refs.pop_back();
    onContainerChanged();
    return value;
}

void CardRefList::insert(size_t idx, cardref_param ref)
{
    check(idx <= m_refs.size(), "invalid card index");
    check(!contains(ref), "card already in row");
    check(!ref.isNil(), "nil card");

    m_refs.insert(m_refs.begin()+ssize_t(idx), ref);
    onContainerChanged();
}

void CardRefList::erase(size_t idx)
{
    check(idx < m_refs.size(), "invalid card index");
    m_refs.erase(m_refs.begin()+(ssize_t)idx);
    onContainerChanged();
}

void CardRefList::erase(cardref_param ref)
{
    erase(getIdx(ref));
}

CardRef CardRefList::get(size_t idx) const
{
    check(idx < m_refs.size(), "invalid card index");
    return m_refs[idx];
}

ssize_t CardRefList::getIdx(cardref_param ref) const
{
    for (ssize_t i = 0; i < m_refs.size(); ++i)
    {
        if (m_refs[i] == ref)
            return i;
    }
    return -1;
}

//
// implementation -> multi-ref container functions
//

void CardRefList::insertMulti(size_t idx, card_ref_list_param refs)
{
    check(idx <= m_refs.size(), "invalid card index");
    check(!containsAny(refs), "card already in row");
    check(!isNilAny(refs), "nil card");

    m_refs.insert(m_refs.begin()+ssize_t(idx), refs.begin(), refs.end());
    onContainerChanged();
}


void CardRefList::eraseMulti(card_index_list_param indices)
{
    check(hasIdxAll(indices, m_refs.size()), "invalid card index");

    if (indices.empty())
        return;

    for (auto&& it : indices)
        m_refs[it].reset();

    card_ref_list_t new_refs;
    for (auto&& it : m_refs)
    {
        if (!it.isNil())
            new_refs.push_back(it);
    }
    m_refs.swap(new_refs);
    onContainerChanged();
}

card_ref_list_t CardRefList::popMulti(card_index_list_param indices)
{
    check(hasIdxAll(indices, m_refs.size()), "invalid card index");

    if (indices.empty())
        return {};

    card_ref_list_t new_refs;
    card_ref_list_t removed_refs;

    for (auto&& it : indices)
    {
        auto&& ref = m_refs[it];
        removed_refs.push_back(ref);
        ref.reset();
    }

    for (auto&& it : m_refs)
    {
        if (!it.isNil())
            new_refs.push_back(it);
    }

    m_refs.swap(new_refs);
    onContainerChanged();

    return removed_refs;
}

card_ref_list_t CardRefList::getMulti(card_index_list_param indices) const
{
    check(hasIdxAll(indices, m_refs.size()), "invalid card index");

    card_ref_list_t card_values;
    for (auto&& it: indices)
        card_values.push_back(m_refs[it]);
    return card_values;
}

card_index_list_t CardRefList::getIndicesMulti(card_ref_list_param refs) const
{
    card_index_list_t indices;
    for (auto&& it: m_refs)
        if (auto idx = getIdx(it))
            if (idx >= 0)
                indices.push_back(idx);
    return indices;
}

