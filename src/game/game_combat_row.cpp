//
// Created by james on 9/27/23.
//

#include "game/game_combat_row.hpp"
using namespace game;

//
// gfx
//

#include "gfx/gfx_draw.hpp"
#include "gfx/gfx_spinner.hpp"
#include "game/asset/assets.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

//
// CombatRow -> Implementation
//

bool CombatRow::tryGetHoveredCard(vec2 const& mp, CardRef& ref)
{
    return
        //cmdr_horn.tryGetHoveredCard(mp, ref) ||
        //weather.tryGetHoveredCard(mp, ref) ||
        units  .tryGetHoveredCard(mp, ref);
}

int CombatRow::updateScore()
{
    int total_strength = units.getTotalStrength();

    if (has_nerf)
    {
        total_strength = (int)units.numCards();
    }

    if (has_buff)
    {
        total_strength *= 2;
    }


    m_score.anim(total_strength);
    return total_strength;
}

void CombatRow::layout(layout::CombatRow const& l)
{
    //cmdr_horn.layout(l.buff);
    //weather.layout(l.nerf);

    m_rect_buff = l.buff;
    m_rect_nerf = l.nerf;

    units.layout(l.units);
    m_score.layout(l.score.anchorCCtoCC(l.score.size()/2));
}

void CombatRow::update(float dt)
{
    m_score.update(dt);
}

void CombatRow::drawAboveCards()
{
    //cmdr_horn.drawAboveCards();
    //weather.drawAboveCards();
    units.drawAboveCards();
}

void CombatRow::drawUnderCards()
{
    //cmdr_horn.drawUnderCards();
    //weather.drawUnderCards();
    units.drawUnderCards();

    m_score.draw();


    drawTextCCtoCC(m_rect_nerf, "nerf", has_nerf ? colors::orangered : colors::dimgray);
    drawTextCCtoCC(m_rect_buff, "buff", has_buff ? colors::lightcyan : colors::dimgray);
}