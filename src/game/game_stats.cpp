//
// Created by james on 9/24/23.
//

#include "game_stats.hpp"
using namespace game;

#include "gfx/gfx_prototype.hpp"
using namespace gfx;

//
// Stats -> Implementation
//

Stats::Stats()
{
    m_spin_score.anim(23);
    m_spin_gems.anim(1);
}

void Stats::updateScore(int score)
{
    m_spin_score.anim(score);
}

void Stats::layout (layout::StatsBoard const& layout)
{
    m_layout = layout;
    m_spin_gems.layout(layout.gems.anchorCCtoCC(layout.gems.size()/2));
    m_spin_score.layout(layout.score.anchorCCtoCC(layout.score.size()/2));
}

void Stats::update (float dt)
{
    m_spin_gems.update(dt);
    m_spin_score.update(dt);
}
void Stats::drawAboveCards()
{

}

void Stats::drawBg(ut::rect const& r, ut::color const& c)
{
    gfx::drawShadow(r, {2,2}, 5);
    gfx::drawRect(r, c);
}

void Stats::drawUnderCards()
{
    PROTO.drawPanel1(m_layout.bounds);
    PROTO.drawAvatar(m_layout.avatar, avatar_texture);

    gfx::drawTextTCtoTC(font, m_layout.name     , name, ut::color(134, 106, 68));
    gfx::drawTextTCtoTC(font, m_layout.deck_name, deck_name, ut::color(134, 106, 68));
    gfx::drawTextTCtoTC(font, m_layout.lead_name, lead_name, ut::color(134, 106, 68));

    m_spin_gems.draw();
    m_spin_score.draw();
}

void Stats::drawDebug()
{

}