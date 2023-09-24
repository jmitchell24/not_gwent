#include "game/game.hpp"

#include <utility>

using namespace game;

#include "assert.hpp"

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "gfx/gfx_draw.hpp"
#include "game/conv.hpp"
#include "game/assets.hpp"

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
// PlayerBoard -> Implementation
//

void PlayerBoard::layout(rect const& bounds)
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

void PlayerBoard::update()
{
    assert(m_is_layout_ready);
}

void PlayerBoard::draw()
{
    assert(m_is_layout_ready);

    drawShadow(m_bounds, {5,5}, 5);
    VIRT.drawRectangle(m_bounds, colors::saddlebrown);

    auto font = game::fonts::smallburgRegular64();

    VIRT.drawTextLCtoLC(font, m_bounds_name, m_name, colors::white);
    VIRT.drawTextLCtoLC(font, m_bounds_deckname, m_deckname, colors::white);
    VIRT.drawTextLCtoLC(font, m_bounds_gems_name, "gems", colors::white);
    VIRT.drawTextCCtoCC(font, m_bounds_handcount_name, "hand", colors::white);
    VIRT.drawTextRCtoRC(font, m_bounds_score_name, "score", colors::white);

    switch (m_gems)
    {
        case GEMS_NONE: VIRT.drawTextLCtoLC(font, m_bounds_gems, "x x", colors::white); break;
        case GEMS_ONE : VIRT.drawTextLCtoLC(font, m_bounds_gems, "o x", colors::white); break;
        case GEMS_TWO : VIRT.drawTextLCtoLC(font, m_bounds_gems, "o o", colors::white); break;
    }

    VIRT.drawTextCCtoCC(font, m_bounds_handcount, PRINTER("%02d", m_handcount), colors::white);
    VIRT.drawTextRCtoRC(font, m_bounds_score, PRINTER("%02d", m_score), colors::white);

    drawShadow(m_bounds_avatar, {1,1}, 3);
    VIRT.drawRectangle(m_bounds_avatar, colors::maroon);


    drawShadow(m_bounds_leader, {1,1}, 3);
    VIRT.drawRectangle(m_bounds_leader, colors::maroon);



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
    m_score = RNG.nexti(100);

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void CombatRow::update()
{
    assert(m_is_layout_ready);

    units.update();
    special.update();
}

void CombatRow::draw()
{
    assert(m_is_layout_ready);

    auto font = game::fonts::smallburgRegular64();
    {
        auto c = colors::hsluv::orchid().scheme<3>();
        auto fg = c[0].withL(85).toColor();
        auto bg = c[0].withL(15).toColor();

        VIRT.drawRectangle(m_bounds_score, bg);
        VIRT.drawTextCCtoCC(font, m_bounds_score.anchorCCtoCC(25, 25), PRINTER("%02d", m_score), fg);

        VIRT.drawRectangle(units.bounds(), c[1].withL(15).toColor());
        VIRT.drawRectangle(special.bounds(), c[2].withL(15).toColor());
    }

    units.draw();
    special.draw();
}

//
// PlayerCards -> Implementation
//

void PlayerCards::layout(rect const& bounds)
{
    m_bounds = bounds;
    graveyard   .layout(m_bounds.row(7, 1, {.w=5, .inner_pad=VIRT_PAD}));
    hand        .layout(m_bounds.row(7, 0, {.inner_pad=VIRT_PAD}));
    deck        .layout(m_bounds.row(7, 6, {.inner_pad=VIRT_PAD}));

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void PlayerCards::update()
{
    assert(m_is_layout_ready);

    hand.update();
    deck.update();
    graveyard.update();
}

void PlayerCards::draw()
{
    assert(m_is_layout_ready);

    hand.draw();
    deck.draw();
    graveyard.draw();
}

//
// WeatherBoard -> Implementation
//

void WeatherBoard::layout(rect const& bounds)
{
    m_bounds = bounds;
    cards.layout(m_bounds);

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void WeatherBoard::update()
{
    assert(m_is_layout_ready);
    cards.update();
}

void WeatherBoard::draw()
{
    assert(m_is_layout_ready);
    cards.draw();
}

//
// GameBoard -> Implementation
//

void GameBoard::layout(rect const& bounds)
{
    m_bounds = bounds;

    auto [b_stats, b_cards_decks] = bounds.shrunk(0, 0, 0, 30).splitLeft(bounds.width() / 4, VIRT_PAD);
    auto [b_cards, b_decks] = b_cards_decks.splitRight(bounds.width() / 4, VIRT_PAD);

    //auto b_cards_rows = b_cards.splitNV<8>(VIRT_PAD);

    m_cards_cpu        .layout(b_cards.col(8, 0, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_siege .layout(b_cards.col(8, 1, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_ranged.layout(b_cards.col(8, 2, {.inner_pad=VIRT_PAD}));
    m_combat_cpu_melee .layout(b_cards.col(8, 3, {.inner_pad=VIRT_PAD}));
    m_combat_usr_siege .layout(b_cards.col(8, 4, {.inner_pad=VIRT_PAD}));
    m_combat_usr_ranged.layout(b_cards.col(8, 5, {.inner_pad=VIRT_PAD}));
    m_combat_usr_melee .layout(b_cards.col(8, 6, {.inner_pad=VIRT_PAD}));
    m_cards_usr        .layout(b_cards.col(8, 7, {.inner_pad=VIRT_PAD}));

    auto [b_stats_cpu, b_weather, b_stats_usr] = b_stats.splitNV<3>(VIRT_PAD);
    m_stats_cpu     .layout(b_stats_cpu);
    m_weather_cards .layout(b_weather);
    m_stats_usr     .layout(b_stats_usr);

    m_card_mover.set({
        &m_combat_cpu_siege.units,
        &m_combat_cpu_siege.special,
        &m_combat_cpu_ranged.units,
        &m_combat_cpu_ranged.special,
        &m_combat_cpu_melee.units,
        &m_combat_cpu_melee.special,
        &m_cards_cpu.hand,
        &m_cards_cpu.deck,
        &m_cards_cpu.graveyard,
        &m_combat_usr_siege.units,
        &m_combat_usr_siege.special,
        &m_combat_usr_ranged.units,
        &m_combat_usr_ranged.special,
        &m_combat_usr_melee.units,
        &m_combat_usr_melee.special,
        &m_cards_usr.hand,
        &m_cards_usr.deck,
        &m_cards_usr.graveyard

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

    m_cards_usr.update();
    m_cards_cpu.update();

    m_stats_cpu.update();
    m_stats_usr.update();
    m_weather_cards.update();
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

    m_cards_usr.draw();
    m_cards_cpu.draw();

    m_stats_cpu.draw();
    m_stats_usr.draw();
    m_weather_cards.draw();

    m_card_mover.draw();
}





































