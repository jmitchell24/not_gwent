//
// Created by james on 9/24/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_draw.hpp"
#include "game/asset/assets.hpp"

namespace game
{
    class Stats
    {
    public:
        std::string name            = "Triss";
        std::string deck_name       = "Northern Realms";
        std::string lead_name       = "Foltest";

        Font        font            = fonts::smallburgRegular128();
        Texture2D   avatar_texture  = textures::avatar_triss();

        Stats();

        void updateScore(int score);

        void layout (layout::StatsBoard const& layout);
        void update (float dt);
        void drawAboveCards();
        void drawBg(ut::rect const& r, ut::color const& c);
        void drawUnderCards();
        void drawDebug();

    private:
        layout::StatsBoard  m_layout;
        gfx::Spinner        m_spin_gems { gfx::Spinner::VERT, ut::color(134, 106, 68) };
        gfx::Spinner        m_spin_score{ gfx::Spinner::VERT, ut::color(134, 106, 68) };
    };
}