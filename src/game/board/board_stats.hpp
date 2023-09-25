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
        Font font;
        Texture2D avatar_texture;

        void layout (layout::StatsBoard const& b)
        {
            m_bound             = b.bounds;
            m_bound_name        = b.name;
            m_bound_deck_name   = b.deck_name;
            m_bound_lead_name   = b.lead_name;
            m_bound_avatar      = b.avatar.fitAspect(1).shrunk(10);

            m_spin_gems.layout(b.gems.anchorCCtoCC(b.gems.size()/2));
            m_spin_score.layout(b.score.anchorCCtoCC(b.score.size()/2));

            font = fonts::smallburgRegular128();
            avatar_texture = textures::avatar_triss();

            m_spin_score.anim(23);
            m_spin_gems.anim(1);
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

            drawBg(m_bound, ut::colors::chocolate);
            drawBg(m_bound_avatar, ut::colors::darkseagreen);


            gfx::drawTextTCtoTC(font, m_bound_name, "Triss", ut::colors::white);
            gfx::drawTextTCtoTC(font, m_bound_deck_name, "Northern Realms", ut::colors::white);
            gfx::drawTextTCtoTC(font, m_bound_lead_name, "Foltest", ut::colors::white);

            m_spin_gems.draw();
            m_spin_score.draw();

            gfx::drawTexture(avatar_texture, m_bound_avatar, ut::colors::white);

        }

        void drawDebug()
        {

        }

    //private:
        ut::rect m_bound;
        ut::rect m_bound_name;
        ut::rect m_bound_deck_name;
        ut::rect m_bound_lead_name;
        ut::rect m_bound_avatar;



        std::string m_name;
        std::string m_deck_name;
        std::string m_lead_name;

        gfx::Spinner m_spin_gems{ gfx::Spinner::HORZ, ut::colors::burlywood };
        gfx::Spinner m_spin_score{ gfx::Spinner::HORZ, ut::colors::burlywood };
    };
}