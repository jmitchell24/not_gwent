//
// Created by james on 10/1/23.
//


#pragma once

#include "scene.hpp"
#include "gfx/gfx_prototype.hpp"
#include "game/layout/board_layout2.hpp"

class SceneProtoTest : public Scene
{
public:

    ut::rect bounds;
    game::layout::GameBoard gb;
    game::layout::GraphicsLayout gl;

    [[nodiscard]] ut::cstrview name() const override
    {
        return "Proto Test";
    }

    void layout() override
    {
        bounds = Context::VIEW_RECT;
        gb.layout(bounds);
        gl.layout(gb);
    }

    Texture2D avatar = game::textures::avatar_leshen();

    void draw() override
    {
        using namespace gfx;



        PROTO.drawWoodBoard(gl.outer, gl.inner);

        PROTO.drawShadow(gl.board_outer);
        PROTO.drawIronFrame(gl.board_outer, gl.board_inner1);
        PROTO.drawWoodBoard(gl.board_inner1, gl.board_inner2);

        PROTO.drawShadow(gl.cards_outer);
        PROTO.drawWoodFlat(gl.cards_inner);
        PROTO.drawIronFrame(gl.cards_outer, gl.cards_inner);

        draw(gl.cpu_melee_buttons);
        draw(gl.cpu_ranged_buttons);
        draw(gl.cpu_siege_buttons);

        draw(gl.usr_melee_buttons);
        draw(gl.usr_ranged_buttons);
        draw(gl.usr_siege_buttons);
    }

    void draw(game::layout::GraphicsLayout::Buttons const& b)
    {
        using namespace gfx;

        PROTO.drawShadow(b.bounds);
        PROTO.drawBuffBoard(b.buff_outer, b.buff_inner);
        PROTO.drawNerfBoard(b.nerf_outer, b.nerf_inner);

        PROTO.drawShadow(b.score_outer);
        PROTO.drawIronFlat(b.score_inner);
        PROTO.drawIronFrame(b.score_outer, b.score_inner);
    }

    void drawDebug() override
    {
        gb.drawDebug();
        gl.drawDebug();



    }
};