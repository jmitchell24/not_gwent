//
// Created by james on 10/1/23.
//


#pragma once

#include "scene.hpp"
#include "gfx/gfx_prototype.hpp"
#include "game/layout/board_layout2.hpp"
#include "ledit/ledit_editor.hpp"

class SceneProtoTest : public Scene
{
public:

    ledit::BoxEditor editor{"graphics"};

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

        editor.setRoot(Context::VIEW_RECT);
    }

    Texture2D avatar = game::textures::avatar_leshen();

    void update(update_param u) override
    {
        editor.view_transform = u.view_transform;
    }

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


        if (editor.draw())
        {
#define TRY_GET_RECT(x_) editor.tryGetBorder(#x_, x_)

            {
                bool b = false;
                b|=TRY_GET_RECT(gl.outer);
                b|=TRY_GET_RECT(gl.inner);

                b|=TRY_GET_RECT(gl.board_outer);
                b|=TRY_GET_RECT(gl.board_inner1);
                b|=TRY_GET_RECT(gl.board_inner2);

                b|=TRY_GET_RECT(gl.cards_outer);
                b|=TRY_GET_RECT(gl.cards_inner);

                b|=TRY_GET_RECT(gl.cpu_siege_buttons.score_outer);
                b|=TRY_GET_RECT(gl.cpu_siege_buttons.score_inner);
                b|=TRY_GET_RECT(gl.cpu_siege_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.cpu_siege_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.cpu_siege_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.cpu_siege_buttons.buff_inner);

                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.score_outer);
                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.score_inner);
                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.cpu_ranged_buttons.buff_inner);

                b|=TRY_GET_RECT(gl.cpu_melee_buttons.score_outer);
                b|=TRY_GET_RECT(gl.cpu_melee_buttons.score_inner);
                b|=TRY_GET_RECT(gl.cpu_melee_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.cpu_melee_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.cpu_melee_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.cpu_melee_buttons.buff_inner);

                b|=TRY_GET_RECT(gl.usr_siege_buttons.score_outer);
                b|=TRY_GET_RECT(gl.usr_siege_buttons.score_inner);
                b|=TRY_GET_RECT(gl.usr_siege_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.usr_siege_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.usr_siege_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.usr_siege_buttons.buff_inner);

                b|=TRY_GET_RECT(gl.usr_ranged_buttons.score_outer);
                b|=TRY_GET_RECT(gl.usr_ranged_buttons.score_inner);
                b|=TRY_GET_RECT(gl.usr_ranged_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.usr_ranged_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.usr_ranged_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.usr_ranged_buttons.buff_inner);

                b|=TRY_GET_RECT(gl.usr_melee_buttons.score_outer);
                b|=TRY_GET_RECT(gl.usr_melee_buttons.score_inner);
                b|=TRY_GET_RECT(gl.usr_melee_buttons.nerf_outer);
                b|=TRY_GET_RECT(gl.usr_melee_buttons.nerf_inner);
                b|=TRY_GET_RECT(gl.usr_melee_buttons.buff_outer);
                b|=TRY_GET_RECT(gl.usr_melee_buttons.buff_inner);
            }


#undef TRY_GET_RECT
        }
    }
};