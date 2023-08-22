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

#define CELL(x_, y_, w_, h_) ( bounds.cell(3,6, x_,y_,  { .w=w_, .h=h_, .inner_pad=VIRT_PAD, .outer_pad=VIRT_PAD}) )
    m_bounds_name           = CELL(0,0, 3,1);
    m_bounds_deckname       = CELL(0,1, 3,1);

    m_bounds_gems_name      = CELL(0,2, 1,1);
    m_bounds_handcount_name = CELL(1,2, 1,1);
    m_bounds_score_name     = CELL(2,2, 1,1);

    m_bounds_gems           = CELL(0,3, 1,1);
    m_bounds_handcount      = CELL(1,3, 1,1);
    m_bounds_score          = CELL(2,3, 1,1);

    m_bounds_avatar         = CELL(0,4, 1,2);
    m_bounds_leader         = CELL(2,4, 1,2);
#undef CELL

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
    VIRT.drawTextLCtoLC(m_bounds_deckname, m_deckname, colors::white);
    VIRT.drawTextLCtoLC(m_bounds_gems_name, "gems", colors::white);
    VIRT.drawTextCCtoCC(m_bounds_handcount_name, "hand", colors::white);
    VIRT.drawTextRCtoRC(m_bounds_score_name, "score", colors::white);

    switch (m_gems)
    {
        case GEMS_NONE: VIRT.drawTextLCtoLC(m_bounds_gems, "x x", colors::white); break;
        case GEMS_ONE : VIRT.drawTextLCtoLC(m_bounds_gems, "o x", colors::white); break;
        case GEMS_TWO : VIRT.drawTextLCtoLC(m_bounds_gems, "o o", colors::white); break;
    }

    VIRT.drawTextCCtoCC(m_bounds_handcount, PRINTER("%02d", m_handcount), colors::white);
    VIRT.drawTextRCtoRC(m_bounds_score, PRINTER("%02d", m_score), colors::white);
    VIRT.drawRectangleLines(m_bounds_avatar, 2.0f, colors::white);
    VIRT.drawRectangleLines(m_bounds_leader, 2.0f, colors::white);

    VIRT.drawRectangleLines(m_bounds, 2.0f, colors::white);

    VIRT_DEBUG(m_bounds_name           );
    VIRT_DEBUG(m_bounds_deckname       );
    VIRT_DEBUG(m_bounds_gems_name      );
    VIRT_DEBUG(m_bounds_handcount_name );
    VIRT_DEBUG(m_bounds_score_name     );
    VIRT_DEBUG(m_bounds_gems           );
    VIRT_DEBUG(m_bounds_handcount      );
    VIRT_DEBUG(m_bounds_score          );
    VIRT_DEBUG(m_bounds_avatar         );
    VIRT_DEBUG(m_bounds_leader         );
}

//
// CombatRow -> Implementation
//

void CombatRow::layout(rect const &bounds)
{
    m_bounds = bounds;

    auto [b_score, b_special_unit] = bounds.splitLeft(50, VIRT_PAD);
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


    {
        auto c = colors::hsluv::orchid().scheme<3>();
        auto fg = c[0].withL(85).toColor();
        auto bg = c[0].withL(15).toColor();

        VIRT.drawRectangle(m_bounds_score, bg);
        VIRT.drawTextCCtoCC(m_bounds_score.anchorCCtoCC(25, 25), PRINTER("%02d", m_score), fg);

        VIRT.drawRectangle(units.bounds(), c[1].withL(15).toColor());
        VIRT.drawRectangle(special.bounds(), c[2].withL(15).toColor());
    }

    units.draw();
    special.draw();
}


//
// GameBoard -> Implementation
//





void GameBoard::layout(ut::rect const &bounds)
{
    m_bounds = bounds;

    auto [b_stats, b_cards_decks] = bounds.shrunk(0, 0, 0, 30).splitLeft(bounds.width() / 4, VIRT_PAD);
    auto [b_cards, b_decks] = b_cards_decks.splitRight(bounds.width() / 4, VIRT_PAD);

    //auto b_cards_rows = b_cards.splitNV<8>(VIRT_PAD);

    m_hand_cpu         .layout(b_cards.col(8, 0, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_siege .layout(b_cards.col(8, 1, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_ranged.layout(b_cards.col(8, 2, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_melee .layout(b_cards.col(8, 3, {.inner_pad=VIRT_PAD}));
    m_combat_usr_siege .layout(b_cards.col(8, 4, {.inner_pad=VIRT_PAD}));
    m_combat_usr_ranged.layout(b_cards.col(8, 5, {.inner_pad=VIRT_PAD}));
    m_combat_usr_melee .layout(b_cards.col(8, 6, {.inner_pad=VIRT_PAD}));
    m_hand_usr         .layout(b_cards.col(8, 7, {.inner_pad=VIRT_PAD}));

    auto [b_stats_cpu, b_dummy, b_stats_usr] = b_stats.splitNV<3>(VIRT_PAD);
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
        &m_hand_usr,
        &m_hand_cpu
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

    m_hand_usr.update();
    m_hand_cpu.update();

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

    m_hand_usr.draw();
    m_hand_cpu.draw();

    m_stats_cpu.draw();
    m_stats_usr.draw();

    m_card_mover.draw();
}





































