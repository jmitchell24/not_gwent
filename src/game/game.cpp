#include "game/game.hpp"

#include <utility>
using namespace game;

#include "random.hpp"
#include "assert.hpp"

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "game/conv.hpp"

using namespace gfx;

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

size_t constexpr static VIRT_WIDTH  = 720;
size_t constexpr static VIRT_HEIGHT = 1280;
size_t constexpr static VIRT_PAD    = 10;



//
// CardHold -> Implementation
//




//
// StatDisplay -> Implementation
//

StatDisplay::StatDisplay(cstrparam player_name) :
    m_player_name   {player_name}
{}

void StatDisplay::onLayout(rect const& bounds)
{
    m_bounds_stats = bounds.shrunk(VIRT_PAD).splitNV<5>(VIRT_PAD);
}

void StatDisplay::update()
{
    assert(isLayoutReady());
}

void StatDisplay::draw()
{
    assert(isLayoutReady());

    VIRT_DEBUG(StatDisplay::m_bounds);

    VIRT.drawTextTLtoTL(m_bounds_stats[0], "player:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[0], m_player_name, colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[1], "gems: "_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[1], PRINTER("%d", gems()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[2], "score:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[2], PRINTER("%d", score()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[3], "library:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[3], PRINTER("%d", librarySize()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[4], "graveyard:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[4], PRINTER("%d", graveyardSize()), colors::white);

    VIRT.drawRectThinLines(m_bounds, colors::white);
}

void StatDisplay::load()
{

}

//
// CombatRow -> Implementation
//

void CombatRow::layout(rect const& bounds)
{
    m_bounds = bounds;

    auto [b_special, b_unit] = bounds.splitLeft(100, VIRT_PAD);

    units.layout(b_unit);
    special.layout(b_special);

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void CombatRow::update()
{
    units.update();
    special.update();
}

void CombatRow::draw()
{
    units.draw();
    special.draw();
}


//
// GameBoard -> Implementation
//





void GameBoard::layout(ut::rect const& bounds)
{
    m_bounds = bounds;

    auto [b_stats, b_cards_decks] = bounds.splitLeft(bounds.width()/5);
    auto [b_cards, b_decks] = b_cards_decks.splitRight(bounds.width()/5);

    auto [b_cards_combat, b_cards_hand] = b_cards.splitBottom(b_cards.height()/5);

    auto b_cards_combat_rows = b_cards_combat.splitNV<6>(VIRT_PAD);

    m_combat_cpu_siege .layout(b_cards_combat_rows[0]);
    m_combat_cpu_ranged.layout(b_cards_combat_rows[1]);
    m_combat_cpu_melee .layout(b_cards_combat_rows[2]);
    m_combat_usr_siege .layout(b_cards_combat_rows[3]);
    m_combat_usr_ranged.layout(b_cards_combat_rows[4]);
    m_combat_usr_melee .layout(b_cards_combat_rows[5]);

    m_card_mover.set({
        &m_combat_cpu_siege .units,
        &m_combat_cpu_siege .special,
        &m_combat_cpu_ranged.units,
        &m_combat_cpu_ranged.special,
        &m_combat_cpu_melee .units,
        &m_combat_cpu_melee .special,
        &m_combat_usr_siege .units,
        &m_combat_usr_siege .special,
        &m_combat_usr_ranged.units,
        &m_combat_usr_ranged.special,
        &m_combat_usr_melee .units,
        &m_combat_usr_melee .special
    });



#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void GameBoard::update()
{
    assert(m_is_layout_ready);

    m_card_mover.update();

    m_combat_cpu_siege .update();
    m_combat_cpu_ranged.update();
    m_combat_cpu_melee .update();
    m_combat_usr_siege .update();
    m_combat_usr_ranged.update();
    m_combat_usr_melee .update();
}

void GameBoard::draw()
{
    assert(m_is_layout_ready);
    m_combat_cpu_siege .draw();
    m_combat_cpu_ranged.draw();
    m_combat_cpu_melee .draw();
    m_combat_usr_siege .draw();
    m_combat_usr_ranged.draw();
    m_combat_usr_melee .draw();

    m_card_mover.draw();
}





































