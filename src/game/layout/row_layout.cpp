//
// Created by james on 9/16/23.
//

#include "game/layout/row_layout.hpp"
using namespace game;

#include "assert.hpp"

//
// ut
//
using namespace ut;

//
// CardList::CardLayout -> Implementation
//

RowLayout RowLayout::create(rect const &bounds, float card_width, size_t card_count)
{
    if (float w = card_width * float(card_count); w < bounds.width())
    {
        return {bounds.anchorCCtoCC(w, bounds.height()), card_count, card_width, card_width};
    }

    return {bounds, card_count, card_width, (bounds.width() - card_width) / float(card_count - 1)};
}

bool RowLayout::tryGetIndex(vec2 const &mp, size_t &idx) const
{
    if (bounds.contains(mp))
    {
        auto mp_min = bounds.min.x;
        auto mp_max = bounds.max.x;
        auto mp_width = mp_max - mp_min;

        idx = size_t(((mp.x - mp_min) / (mp_width)) * float(card_count));
        return true;
    }
    return false;
}

vec2 RowLayout::getPos(size_t idx) const
{
    return bounds.pos().withOffsetX(card_gap * float(idx));
}

rect RowLayout::getRect(size_t idx) const
{
    return bounds.withWidth(card_width).withOffsetX(card_gap * float(idx));
}

CardLayout RowLayout::getCardLayout(size_t idx) const
{
    auto r = getRect(idx);
    return { r.width(), r.height(), r.x(), r.y(), 1.0f };
}
