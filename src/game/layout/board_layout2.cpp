//
// Created by james on 9/11/23.
//

#include "game/layout/board_layout2.hpp"
#include "game/layout/card_layout.hpp"
using namespace game_layout;



#include "gfx/gfx_drect.hpp"


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
// GraphicsLayout
//

void GraphicsLayout::layout(GameBoard const& gb)
{
    outer = gb.bounds;
    inner = outer.deflated(20);

    board_inner2 = { gb.cpu.siege.bounds.min-20, gb.usr.siege.bounds.max+20 };
    board_inner1 = board_inner2.inflated(20);
    board_outer  = board_inner1.inflated(10);

    cards_inner = { gb.cpu.siege.units.min, gb.usr.siege.units.max };
    cards_outer = cards_inner.inflated(10);

    cpu_siege_buttons   .layout(gb.cpu.siege);
    cpu_ranged_buttons  .layout(gb.cpu.ranged);
    cpu_melee_buttons   .layout(gb.cpu.melee);

    usr_melee_buttons   .layout(gb.usr.siege);
    usr_ranged_buttons  .layout(gb.usr.ranged);
    usr_siege_buttons   .layout(gb.usr.melee);
}

void GraphicsLayout::drawDebug()
{
    DRECT_PUSH2(GraphicsLayout,outer);
    DRECT_PUSH1(inner);

    DRECT1(board_outer);
    DRECT1(board_inner1);
    DRECT1(board_inner2);

    cpu_siege_buttons   .drawDebug();
    cpu_ranged_buttons  .drawDebug();
    cpu_melee_buttons   .drawDebug();

    usr_melee_buttons   .drawDebug();
    usr_ranged_buttons  .drawDebug();
    usr_siege_buttons   .drawDebug();

    DRECT_POP();
    DRECT_POP();
}


//
// GraphicsLayout::Buttons
//

void GraphicsLayout::Buttons::layout(CombatRow const& row)
{


    buff_outer = row.buff;
    buff_inner = buff_outer.deflated(5,5,5,2.5);

    nerf_outer = row.nerf;
    nerf_inner = nerf_outer.deflated(5,2.5,5,5);

    score_outer = row.score;
    score_inner = score_outer.deflated(5);

    bounds = { buff_outer.min, nerf_outer.max };
}

void GraphicsLayout::Buttons::drawDebug()
{
    DRECT_PUSH2(GraphicsLayout::Buttons,bounds);

    DRECT1(buff_outer);
    DRECT1(buff_inner);
    DRECT1(nerf_outer);
    DRECT1(nerf_inner);



    DRECT_POP();
}


