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
        std::string title = "Card Picker";
        game_layout::CardPicker         m_layout;
        bool isOpen() const;
        void open(cardrefs_param refs);
        bool tryClose(ut::vec2 const& mp, CardRef& ref);

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);

        void layout();
        void update(update_param u);
        void drawUnderCards();
        void drawAboveCards();
        void drawDebug();

    private:
        bool             m_open=false;
        cardrefs_t       m_refs;
        board::BoardRow  m_row_options;
        board::BoardSlot m_slot_choice;

        void close();
    };
}