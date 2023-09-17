//
// Created by james on 9/11/23.
//

#include "board_layout.hpp"
using namespace game::layout;



#include "gfx/gfx_virt2d.hpp"


//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

size_t constexpr static VIRT_WIDTH      = 720;
size_t constexpr static VIRT_HEIGHT     = 1280;
size_t constexpr static VIRT_PAD        = 10;

//
// CombatRow
//

void CombatRow::layout(rect const& b)
{
    auto [b_score, b_special_unit] = b.splitLeft(50, VIRT_PAD);
    auto [b_special, b_unit] = b_special_unit.splitLeft(80, VIRT_PAD);

    bounds      = b;
    score       = b_score;
    special     = b_special;
    units       = b_unit;
}

void CombatRow::drawDebug()
{
    DRECT_PUSH2(CombatRow,bounds);
    DRECT1(score);
    DRECT1(special);
    DRECT1(units);
    DRECT_POP();
}

//
// PlayerRow
//

void PlayerRow::layout(rect const& b)
{
    bounds    = b;
    deck      = b.row(7, 0, {.inner_pad=VIRT_PAD});
    hand      = b.row(7, 1, {.w=5, .inner_pad=VIRT_PAD});
    graveyard = b.row(7, 6, {.inner_pad=VIRT_PAD});
}

void PlayerRow::drawDebug()
{
    DRECT_PUSH2(PlayerRow,bounds);
    DRECT1(hand);
    DRECT1(deck);
    DRECT1(graveyard);
    DRECT_POP();
}

//
// WeatherBoard
//

void WeatherBoard::layout(rect const& b)
{
    bounds = b;
}

void WeatherBoard::drawDebug()
{
    DRECT2(WeatherBoard,bounds);
}

//
// PlayerBoard
//

void StatsBoard::layout(rect const& b)
{
    bounds = b;

#define CELL(x_, y_, w_, h_) ( b.cell(3,6, x_,y_,  { .w=w_, .h=h_, .inner_pad=VIRT_PAD, .outer_pad=VIRT_PAD}) )
    name                = CELL(0,0, 3,1);
    deck_name           = CELL(0,1, 3,1);

    gems_label          = CELL(0,2, 1,1);
    hand_count_label    = CELL(1,2, 1,1);
    score_label         = CELL(2,2, 1,1);

    gems                = CELL(0,3, 1,1);
    hand_count          = CELL(1,3, 1,1);
    score               = CELL(2,3, 1,1);

    avatar              = CELL(0,4, 1,2);
    leader              = CELL(2,4, 1,2);
#undef CELL
}

void StatsBoard::drawDebug()
{
    DRECT_PUSH2(StatsBoard,bounds);
    DRECT1(name);
    DRECT1(deck_name);
    DRECT1(gems_label);
    DRECT1(hand_count_label);
    DRECT1(score_label);
    DRECT1(gems);
    DRECT1(hand_count);
    DRECT1(score);
    DRECT1(avatar);
    DRECT1(leader);
    DRECT_POP();
}

//
// GameBoard
//

void GameBoard::layout(rect const& b)
{
    auto [b_stats, b_cards_decks] = b.shrunk(0, 0, 0, 30).splitLeft(b.width() / 4, VIRT_PAD);
    auto [b_cards, b_decks] = b_cards_decks.splitRight(b.width() / 4, VIRT_PAD);
    auto [b_stats_cpu, b_weather, b_stats_usr] = b_stats.splitNV<3>(VIRT_PAD);

    bounds = b;
    player_cpu       .layout(b_cards.col(8, 0, {.inner_pad=VIRT_PAD}));
    combat_cpu_siege .layout(b_cards.col(8, 1, {.inner_pad=VIRT_PAD}));
    combat_cpu_ranged.layout(b_cards.col(8, 2, {.inner_pad=VIRT_PAD}));
    combat_cpu_melee .layout(b_cards.col(8, 3, {.inner_pad=VIRT_PAD}));
    combat_usr_siege .layout(b_cards.col(8, 4, {.inner_pad=VIRT_PAD}));
    combat_usr_ranged.layout(b_cards.col(8, 5, {.inner_pad=VIRT_PAD}));
    combat_usr_melee .layout(b_cards.col(8, 6, {.inner_pad=VIRT_PAD}));
    player_usr       .layout(b_cards.col(8, 7, {.inner_pad=VIRT_PAD}));
    stats_cpu        .layout(b_stats_cpu);
    weather          .layout(b_weather);
    stats_usr        .layout(b_stats_usr);
}

void GameBoard::drawDebug()
{
    DRECT_PUSH2(GameBoard,bounds);
    player_cpu       .drawDebug();
    combat_cpu_siege .drawDebug();
    combat_cpu_ranged.drawDebug();
    combat_cpu_melee .drawDebug();
    combat_usr_siege .drawDebug();
    combat_usr_ranged.drawDebug();
    combat_usr_melee .drawDebug();
    player_usr       .drawDebug();
    stats_cpu        .drawDebug();
    weather          .drawDebug();
    stats_usr        .drawDebug();
    DRECT_POP();
}



