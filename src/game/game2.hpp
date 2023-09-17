//
// Created by james on 9/15/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "game/layout/row_layout.hpp"
#include "game/layout/card_row.hpp"
#include "game/card_tank.hpp"
#include "gfx/gfx_spinner.hpp"

namespace game
{
    class CardRow
    {
    public:
        ut::rect bounds;

        cardidlist_t        ids;
        layout::RowLayout   layout_row;
        layout::RowLayout   layout_row_next;

        inline bool containsID(cardid_t id) { return getIdx(id) >= 0; }

        void layout(ut::rect const& b)
        {
            bounds = b;
            rebuildLayout();
        }

        void addID(size_t idx, cardid_t id)
        {
            assert(idx <= ids.size());
            ids.insert(ids.begin()+ssize_t(idx), id);
            rebuildLayout();
        }

        void removeID(size_t idx)
        {
            assert(idx < ids.size());
            ids.erase(ids.begin() + ssize_t(idx));
            rebuildLayout();
        }

        void removeID(cardid_t id)
        {
            ssize_t idx = getIdx(id);
            assert(idx >= 0);
            removeID(size_t(idx));
        }

    private:
        ssize_t getIdx(cardid_t id)
        {
            for (size_t i = 0; i < ids.size(); ++i)
                if (ids[i] == id)
                    return i;
            return -1;
        }

        void rebuildLayout()
        {
            layout_row = layout::RowLayout::create(
                    layout::RowLayout::DIR_CENTER,
                    bounds,
                    layout::CardLayout::widthFromHeight(bounds.height()),
                    ids.size());

            layout_row_next = layout::RowLayout::create(
                    layout::RowLayout::DIR_CENTER, bounds,
                    layout::CardLayout::widthFromHeight(bounds.height()),
                    ids.size()+1);
        }
    };

    struct GameBoard2
    {
        CardTank tank;

        CardRow         row1;
        gfx::Spinner    spinner;
        CardRow         row2;

        layout::GameBoard gb;

        void layout(ut::rect const& bounds);
        void update(float dt);
        void draw();
        void drawDebug();
    };
}