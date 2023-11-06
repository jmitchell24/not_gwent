//
// Created by james on 9/11/23.
//

#include  "game/layout/board_layout.hpp"
#include "game/layout/card_layout.hpp"
using namespace game::layout;



#include "gfx/gfx_drect.hpp"

//
// ledit
//
#include "ledit/ledit_editor.hpp"

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

static ledit::BoxEditor BOX_EDITOR {"GameBoard"};

//
// CombatRow
//

void CombatRow::layout(rect const& b)
{
    //auto [b_score, b_cards] = b.splitLeft(50, VIRT_PAD);

    bounds = b;
//    score       = b_score;
//    cmdr_horn   = b_cards.row(8, 0, {       .inner_pad=VIRT_PAD });
//    units       = b_cards.row(8, 1, { .w=6, .inner_pad=VIRT_PAD });
//    weather     = b_cards.row(8, 7, {       .inner_pad=VIRT_PAD });


    score     = bounds.row(8, 0, {       .inner_pad=30 }).fitAspect(1);
    units     = bounds.row(8, 1, { .w=6, .inner_pad=30 });

    auto mods = bounds.row(8, 7, {       .inner_pad=30 }).fitAspect(1);
    buff    = mods.col(2, 0);
    nerf    = mods.col(2, 1);
}

void CombatRow::drawDebug()
{
    DRECT_PUSH2(CombatRow,bounds);
    DRECT1(score);
    DRECT1(buff);
    DRECT1(nerf);
    DRECT1(units);
    DRECT_POP();
}

//
// PlayerRow
//

void PlayerRow::layout(rect const& b)
{
    bounds  = b;
    lead    = b.row(8, 0, {      .inner_pad=VIRT_PAD});
    deck    = b.row(8, 1, {      .inner_pad=VIRT_PAD});
    hand    = b.row(8, 2, {.w=5, .inner_pad=VIRT_PAD});
    yard    = b.row(8, 7, {      .inner_pad=VIRT_PAD});
}

void PlayerRow::drawDebug()
{
    DRECT_PUSH2(PlayerRow,bounds);
    DRECT1(hand);
    DRECT1(deck);
    DRECT1(yard);
    DRECT1(lead);
    DRECT_POP();
}

//
// WeatherBoard
//

//void WeatherBoard::layout(rect const& b)
//{
//    bounds = b;
//}
//
//void WeatherBoard::drawDebug()
//{
//    DRECT2(WeatherBoard,bounds);
//}

//
// PlayerBoard
//

void StatsBoard::layout(rect const& b)
{
    bounds = b;

#define CELL(x_, y_, w_, h_) ( bounds.cell(10,10, x_,y_,  { .w=(w_), .h=h_, .inner_pad=5, .outer_pad=5}) )
    name                = CELL(0,0, 6,2);
    deck_name           = CELL(0,2, 6,1);
    lead_name           = CELL(0,3, 6,1);
    avatar              = CELL(0,4, 6,6).fitAspect(1).deflated(VIRT_PAD);
    gems                = CELL(6,0, 4,5);
    score               = CELL(6,5, 4,5);
#undef CELL



}

void StatsBoard::drawDebug()
{
    DRECT_PUSH2(StatsBoard,bounds);

    DRECT1(name);
    DRECT1(deck_name);
    DRECT1(lead_name);
    DRECT1(gems);
    DRECT1(score);
    DRECT1(avatar);

    DRECT_POP();
}



//
// GameBoard
//

void GameBoard::layout(rect const& b)
{
    auto [b_stats    , b_cards_decks] = b.deflated(VIRT_PAD).splitLeft(b.width() / 4, VIRT_PAD);
    auto [b_cards    , b_decks      ] = b_cards_decks.splitRight(b.width() / 4, VIRT_PAD);
    auto [b_stats_cpu, b_stats_usr  ] = b_stats.splitNV<2>(150);

    bounds = b;

    cpu.player.layout(b_cards.col(8, 0, {.inner_pad=VIRT_PAD}));
    cpu.siege .layout(b_cards.col(8, 1, {.inner_pad=VIRT_PAD}));
    cpu.ranged.layout(b_cards.col(8, 2, {.inner_pad=VIRT_PAD}));
    cpu.melee .layout(b_cards.col(8, 3, {.inner_pad=VIRT_PAD}));

    usr.melee .layout(b_cards.col(8, 4, {.inner_pad=VIRT_PAD}));
    usr.ranged.layout(b_cards.col(8, 5, {.inner_pad=VIRT_PAD}));
    usr.siege .layout(b_cards.col(8, 6, {.inner_pad=VIRT_PAD}));
    usr.player.layout(b_cards.col(8, 7, {.inner_pad=VIRT_PAD}));

    cpu.stats .layout(b_stats_cpu);
    //weather   .layout(b_weather);
    usr.stats .layout(b_stats_usr);

    //card_size = CardLayout::sizeFromHeight(usr.player.bounds.height());

    BOX_EDITOR.setRoot(b);
}



void GameBoard::drawDebug()
{


    DRECT_PUSH2(GameBoard,bounds);

    DRECT_PUSH2(cpu, cpu.bounds);
    cpu.player.drawDebug();
    cpu.melee .drawDebug();
    cpu.ranged.drawDebug();
    cpu.siege .drawDebug();
    DRECT_POP();

    DRECT_PUSH2(usr, usr.bounds);
    usr.player.drawDebug();
    usr.melee .drawDebug();
    usr.ranged.drawDebug();
    usr.siege .drawDebug();
    DRECT_POP();

    //weather   .drawDebug();
    cpu.stats .drawDebug();
    usr.stats .drawDebug();
    DRECT_POP();

    BOX_EDITOR.view_transform = gfx::DRECT.view_transform;
    if (BOX_EDITOR.draw())
    {
        if (auto box = BOX_EDITOR.getRect("test"))
            usr.stats.avatar = *box;
    }
}



