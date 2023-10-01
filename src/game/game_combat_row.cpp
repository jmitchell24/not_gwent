//
// Created by james on 9/27/23.
//

#include "game/game_combat_row.hpp"
using namespace game;

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
        cmdr_horn.tryGetHoveredCard(mp, ref) ||
        weather.tryGetHoveredCard(mp, ref) ||
        units  .tryGetHoveredCard(mp, ref);
}

int CombatRow::updateScore()
{
    int total_strength = units.getTotalStrength();

    if (!weather.isEmpty())
    {
        total_strength = units.numCards();
    }

    if (!cmdr_horn.isEmpty())
    {
        total_strength *= 2;
    }


    m_score.anim(total_strength);
    return total_strength;
}

void CombatRow::layout(layout::CombatRow const& l)
{
    cmdr_horn.layout(l.cmdr_horn);
    weather.layout(l.weather);
    units.layout(l.units);
    m_score.layout(l.score.anchorCCtoCC(l.score.size()/2));
}

void CombatRow::update(float dt)
{
    m_score.update(dt);
}

void CombatRow::drawAboveCards()
{
    cmdr_horn.drawAboveCards();
    weather.drawAboveCards();
    units.drawAboveCards();
}

void CombatRow::drawUnderCards()
{
    cmdr_horn.drawUnderCards();
    weather.drawUnderCards();
    units.drawUnderCards();

    m_score.draw();
}