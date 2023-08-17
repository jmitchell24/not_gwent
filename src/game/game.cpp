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
#include <ut/random.hpp>

using namespace ut;

//
// std
//
using namespace std;

size_t constexpr static VIRT_WIDTH = 720;
size_t constexpr static VIRT_HEIGHT = 1280;
size_t constexpr static VIRT_PAD = 10;



//
// CardHold -> Implementation
//




//
// PlayerStats -> Implementation
//

void PlayerStats::layout(rect const& bounds)
{
    m_bounds = bounds;

    auto [b_va1, b_va2] = bounds.splitBottom(75, VIRT_PAD);

    auto [b_name, b_deckname, b_number_names, b_numbers] = b_va1.shrunk(VIRT_PAD).splitNV<4>(VIRT_PAD);
    m_bounds_name = b_name;
    m_bounds_deckname = b_deckname;

    auto [b_gems, b_handcount, b_score] = b_numbers.splitNH<3>(VIRT_PAD);
    m_bounds_gems      = b_gems;
    m_bounds_handcount = b_handcount;
    m_bounds_score     = b_score;

    auto [b_gems_name, b_handcount_name, b_score_name] = b_number_names.splitNH<3>(VIRT_PAD);
    m_bounds_gems_name      = b_gems_name;
    m_bounds_handcount_name = b_handcount_name;
    m_bounds_score_name     = b_score_name;

    auto [b_leader, b_avatar] = b_va2.splitNH<2>(VIRT_PAD);
    m_bounds_avatar = b_avatar;
    m_bounds_leader = b_leader;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void PlayerStats::update()
{
    assert(m_is_layout_ready);
}

void PlayerStats::draw()
{
    assert(m_is_layout_ready);

    VIRT.drawTextLCtoLC(m_bounds_name, m_name, colors::white);
    VIRT_DEBUG(m_bounds_name);

    VIRT.drawTextLCtoLC(m_bounds_deckname, m_deckname, colors::white);
    VIRT_DEBUG(m_bounds_deckname);

    VIRT_DEBUG(m_bounds_gems);
    VIRT_DEBUG(m_bounds_handcount);
    VIRT_DEBUG(m_bounds_score);

    VIRT.drawTextLCtoLC(m_bounds_gems_name, "gems", colors::white);
    VIRT.drawTextCCtoCC(m_bounds_handcount_name, "hand", colors::white);
    VIRT.drawTextRCtoRC(m_bounds_score_name, "score", colors::white);


    switch (m_gems)
    {
        case NONE: VIRT.drawTextLCtoLC(m_bounds_gems, "x x", colors::white); break;
        case ONE : VIRT.drawTextLCtoLC(m_bounds_gems, "o x", colors::white); break;
        case TWO : VIRT.drawTextLCtoLC(m_bounds_gems, "o o", colors::white); break;
    }


    VIRT.drawTextCCtoCC(m_bounds_handcount, PRINTER("%02d", m_handcount), colors::white);
    VIRT.drawTextRCtoRC(m_bounds_score, PRINTER("%02d", m_score), colors::white);

    VIRT.drawRectangleLines(m_bounds, 2.0f, colors::white);

    VIRT.drawRectangleLines(m_bounds_avatar, 2.0f, colors::white);
    VIRT.drawRectangleLines(m_bounds_leader, 2.0f, colors::white);
}

//
// CombatRow -> Implementation
//

void CombatRow::layout(rect const &bounds)
{
    m_bounds = bounds;

    auto [b_score, b_special_unit] = bounds.splitLeft(45, VIRT_PAD);
    auto [b_special, b_unit] = b_special_unit.splitLeft(80, VIRT_PAD);

    units.layout(b_unit);
    special.layout(b_special);

    m_bounds_score = b_score;
    m_score = rangen().nexti(100);

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

    VIRT.drawRectangle(m_bounds_score, colors::darkgoldenrod);
    VIRT.drawTextCCtoCC(m_bounds_score.anchorCCtoCC(25, 25), PRINTER("%02d", m_score), colors::ivory);
}


//
// GameBoard -> Implementation
//





void GameBoard::layout(ut::rect const &bounds)
{
    m_bounds = bounds;

    auto [b_stats, b_cards_decks] = bounds.splitLeft(bounds.width() / 4);
    auto [b_cards, b_decks] = b_cards_decks.splitRight(bounds.width() / 4);

    auto [b_cards_combat, b_cards_hand] = b_cards.splitBottom(b_cards.height() / 5);
    m_hand.layout(b_cards_hand.shrunk(VIRT_PAD));

    auto b_cards_combat_rows = b_cards_combat.splitNV<6>(VIRT_PAD);
    m_combat_cpu_siege.layout(b_cards_combat_rows[0]);
    m_combat_cpu_ranged.layout(b_cards_combat_rows[1]);
    m_combat_cpu_melee.layout(b_cards_combat_rows[2]);
    m_combat_usr_siege.layout(b_cards_combat_rows[3]);
    m_combat_usr_ranged.layout(b_cards_combat_rows[4]);
    m_combat_usr_melee.layout(b_cards_combat_rows[5]);

    auto [b_stats_cpu, b_dummy, b_stats_usr] = b_stats.shrunk(VIRT_PAD).splitNV<3>(VIRT_PAD);
    m_stats_cpu.layout(b_stats_cpu);
    m_stats_usr.layout(b_stats_usr);

    m_card_mover.set({
        &m_combat_cpu_siege.units,
        &m_combat_cpu_siege.special,
        &m_combat_cpu_ranged.units,
        &m_combat_cpu_ranged.special,
        &m_combat_cpu_melee.units,
        &m_combat_cpu_melee.special,
        &m_combat_usr_siege.units,
        &m_combat_usr_siege.special,
        &m_combat_usr_ranged.units,
        &m_combat_usr_ranged.special,
        &m_combat_usr_melee.units,
        &m_combat_usr_melee.special,
        &m_hand
    });


#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void GameBoard::update()
{
    assert(m_is_layout_ready);

    m_card_mover.update();

    m_combat_cpu_siege.update();
    m_combat_cpu_ranged.update();
    m_combat_cpu_melee.update();
    m_combat_usr_siege.update();
    m_combat_usr_ranged.update();
    m_combat_usr_melee.update();

    m_hand.update();

    m_stats_cpu.update();
    m_stats_usr.update();
}

void GameBoard::draw()
{
    assert(m_is_layout_ready);

    m_combat_cpu_siege.draw();
    m_combat_cpu_ranged.draw();
    m_combat_cpu_melee.draw();
    m_combat_usr_siege.draw();
    m_combat_usr_ranged.draw();
    m_combat_usr_melee.draw();

    m_hand.draw();

    m_stats_cpu.draw();
    m_stats_usr.draw();

    m_card_mover.draw();
}





































