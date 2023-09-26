//
// Created by james on 9/24/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_draw.hpp"
#include "game/assets.hpp"

namespace game::board
{
    class BoardStats
    {
    public:
        std::string name            = "Triss";
        std::string deck_name       = "Northern Realms";
        std::string lead_name       = "Foltest";

        Font        font            = fonts::smallburgRegular128();
        Texture2D   avatar_texture  = textures::avatar_triss();

        BoardStats()
        {
            m_spin_score.anim(23);
            m_spin_gems.anim(1);
        }

        void layout (layout::StatsBoard const& layout)
        {
            m_layout = layout;
            m_spin_gems.layout(layout.gems.anchorCCtoCC(layout.gems.size()/2));
            m_spin_score.layout(layout.score.anchorCCtoCC(layout.score.size()/2));
        }
        void update (float dt)
        {
            m_spin_gems.update(dt);
            m_spin_score.update(dt);
        }
        void drawAboveCards()
        {

        }

        void drawBg(ut::rect const& r, ut::color const& c)
        {
            gfx::drawShadow(r, {2,2}, 5);
            gfx::drawRect(r, c);
        }

        void drawUnderCards()
        {

            drawBg(m_layout.bounds, ut::color(102, 71, 39));
            drawBg(m_layout.avatar, ut::colors::darkseagreen);


            gfx::drawTextTCtoTC(font, m_layout.name     , name, ut::color(134, 106, 68));
            gfx::drawTextTCtoTC(font, m_layout.deck_name, deck_name, ut::color(134, 106, 68));
            gfx::drawTextTCtoTC(font, m_layout.lead_name, lead_name, ut::color(134, 106, 68));

            m_spin_gems.draw();
            m_spin_score.draw();

            gfx::drawTexture(avatar_texture, m_layout.avatar, ut::colors::white);

        }

        void drawDebug()
        {

        }

    //private:
        layout::StatsBoard m_layout;





        gfx::Spinner m_spin_gems{ gfx::Spinner::HORZ, ut::color(134, 106, 68) };
        gfx::Spinner m_spin_score{ gfx::Spinner::HORZ, ut::color(134, 106, 68) };
    };
}