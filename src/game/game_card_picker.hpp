//
// Created by james on 12/13/23.
//

#pragma once

#include "context.hpp"

#include "game/board/board_boss.hpp"
#include "game/layout/board_layout.hpp"

namespace game
{
    class CardPicker
    {
    public:
        layout::CardPicker cp;
        board::BoardRow row;

        bool wantCaptureMouse() const;
        void open(cardrefs_param refs);
        void close();

        void layout(layout::CardPicker const& cp);
        void update(update_param u);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug();

    private:
        bool m_open=false;
        ut::rect r_button_confirm;
        ut::rect r_button_cancel;
        ut::rect r_title;
        ut::rect r_cards;
        std::string title;
    };
}