//
// Created by james on 9/22/23.
//

#include "game/board/board_boss.hpp"
using namespace game::board;

#include <ut/check.hpp>
using namespace ut;

void BoardBoss::slotToRow(BoardSlot& slot, BoardRow& row, size_t idx)
{
    CardRef ref = slot.giveCard();
    row.addCard(idx, ref);
    TANK.elevateCard(ref.id);
}

void BoardBoss::rowToSlot(BoardRow& row, BoardSlot& slot, size_t idx)
{
    CardRef ref = row.giveCard(idx);
    slot.takeCard(ref);
    TANK.elevateCard(ref.id);
}

void BoardBoss::rowToStack(BoardRow& row, BoardStack& stack, size_t idx)
{
    if (row.isEmpty())
        return;

    CardRef card = row.giveCard(idx);
    stack.pushCard(card);
    TANK.elevateCard(card.id);

    m_terminal_cards.push_back(card);
}

void BoardBoss::rowToStack(BoardRow& row, BoardStack& stack, card_indices_param indices)
{
    if (row.isEmpty()) return;
    if (indices.empty()) return;

    auto refs = row.giveCardMulti(indices);

    for (auto&& it: refs)
    {
        stack.pushCard(it);
        TANK.elevateCard(it.id);
        m_terminal_cards.push_back(it);
    }
}

void BoardBoss::stackToRow(BoardStack& stack, BoardRow& row, size_t idx)
{
    if (stack.empty())
        return;

    CardRef ref = stack.popCard();
    row.addCard(idx, ref);
    TANK.elevateCard(ref.id);
}

void BoardBoss::rowToRow(BoardRow& row_a, BoardRow& row_b, size_t idx_a, size_t idx_b)
{
    assert(idx_a < row_a.numCards());
    assert(idx_b <= row_b.numCards());

    CardRef ref = row_a.giveCard(idx_a);
    row_b.addCard(idx_b, ref);
    TANK.elevateCard(ref.id);
}

void BoardBoss::update(float dt)
{
    cardrefs_t tmp;
    tmp.reserve(m_terminal_cards.size());

    for (auto&& it: m_terminal_cards)
    {
        assert(it);
        if (it->isNotMoving())
            TANK.removeCard(it.id);
        else
            tmp.push_back(it);
    }

    m_terminal_cards.swap(tmp);
}