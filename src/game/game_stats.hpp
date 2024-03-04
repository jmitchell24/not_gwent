//
// Created by james on 9/24/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_draw.hpp"
#include "res/res.hpp"

namespace game
{
    class Stats
    {
    public:
        std::string name            = "Triss";
        std::string deck_name       = "Northern Realms";
        std::string lead_name       = "Foltest";

        Font        font            = res::fonts::smallburgRegular128();
        Texture2D   avatar_texture  = res::textures::avatar_triss();

        Stats();

        void updateScore(int score);

        void layout (game_layout::StatsBoard const& layout);
        void update (float dt);
        void drawUnderCards();
        void drawAboveCards();
        //void drawBg(ut::rect const& r, ut::color const& c);
        void drawDebug();

    private:
        game_layout::StatsBoard     m_layout;
        gfx::Spinner                m_spin_gems     { gfx::Spinner::VERT, ut::color(134, 106, 68) };
        gfx::Spinner                m_spin_score    { gfx::Spinner::VERT, ut::color(134, 106, 68) };
    };
}