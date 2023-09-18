//
// Created by james on 9/15/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "game/layout/row_layout.hpp"
#include "game/layout/card_row.hpp"
#include "game/card_tank.hpp"
#include "game/assets.hpp"

#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_draw.hpp"

namespace game
{
    class CardStack2
    {
    public:
        ut::rect bounds;

        Texture2D card_back = textures::card_back_neutral();

        ng::cardlist_t cards;

        gfx::Spinner spinner{gfx::Spinner::HORZ, ut::colors::burlywood};

        void layout(Texture2D t, ut::rect const& b);
        void update(float dt);
        void draw();
    private:

    };

    class CardRow2
    {
    public:
        ut::rect bounds;

        cardidlist_t        ids;
        layout::RowLayout   layout_row;
        layout::RowLayout   layout_row_next;


        void layout     (ut::rect const& b);
        void addID      (size_t idx, cardid_t id);
        void removeID   (size_t idx);
        void removeID   (cardid_t id);

    private:
        ssize_t getIdx(cardid_t id);

        void rebuildLayout();
    };

    struct GameBoard2
    {
        CardTank tank;

        CardRow2         row1;
        gfx::Spinner    spinner;
        CardRow2         row2;

        CardStack2 card_stack_deck;
        CardStack2 card_stack_graveyard;

        layout::GameBoard gb;

        void layout(ut::rect const& bounds);
        void update(float dt);
        void draw();
        void drawDebug();
    };
}