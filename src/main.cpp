//
// game
//
#include "game/conv.hpp"
#include "game/game.hpp"
using namespace game;

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

//
// std
//
#include <optional>
using namespace std;

#include "raylib.h"
#include "rlgl.h"
#include "rlImGui/rlImGui.h"
#include "rlImGui/imgui/imgui_extra.hpp"
#include "rlImGui/extras/IconsFontAwesome5.h"
//#include <lua.hpp>

size_t constexpr static SCREEN_WIDTH    = 1280;
size_t constexpr static SCREEN_HEIGHT   = 720;

size_t constexpr static VIRT_WIDTH  = 1280;
size_t constexpr static VIRT_HEIGHT = 720;
size_t constexpr static VIRT_PAD    = 10;

int main()
{



    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "not gwent");

    rlImGuiBeginInitImGui();
    rlImGuiAddFontAwesomeIconFonts(9);
    rlImGuiReloadFonts();
    rlImGuiEndInitImGui();


    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    auto window_bounds = rect(0, 0, VIRT_WIDTH, VIRT_HEIGHT).shrunk(VIRT_PAD);

    GameBoard gb;
    gb.layout(window_bounds);


//    lua_State* L = luaL_newstate();
//    luaL_openlibs(L); // Open standard Lua libraries
//
//    int result = luaL_dofile(L, "data/scripts/myscript.lua");
//    if (result != 0) {
//        // Handle error
//    }
//
//    lua_close(L);



    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        VIRT.layout(ImGui::GetDockspaceViewport(), VIRT_WIDTH, VIRT_HEIGHT);
        VIRT.begin();

        if (false)
        {
            auto outer_bounds = window_bounds;

            VIRT_DEBUG(outer_bounds);
            for (size_t i = 0; i < 5; ++i)
            {
                for (size_t j = 0; j < 5; ++j)
                {
                    auto r = outer_bounds.cell(5,5, i,j, {.inner_pad=10.0f, .outer_pad=10.0f});
                    VIRT_DEBUG(r);
                }
            }

            auto ralt = outer_bounds.cell(5,5, 1,1, {.w=2, .h=2, .inner_pad=10.0f, .outer_pad=10.0f});
            VIRT_DEBUG(ralt);
        }

        if (false)
        {
            auto outer_bounds = window_bounds;
            VIRT_DEBUG(outer_bounds);

            rect a,b,c,d,e,f;

            outer_bounds.colTie({.h=2, .inner_pad=10, .outer_pad=10}, a,b,c,d,e,f);

            VIRT_DEBUG(a);
            VIRT_DEBUG(b);
            VIRT_DEBUG(c);
            VIRT_DEBUG(d);
            VIRT_DEBUG(e);
            VIRT_DEBUG(f);



        }


        gb.update();
        gb.draw();


        VIRT.end();

        DrawFPS(10,10);

        DrawGrid(10,10);

        rlImGuiBegin();
        {
            ImGui::RenderDockspace();

            ImGui::Begin("main_window");
            {

                VIRT.drawDebug();
            }
            ImGui::End();
        }
        rlImGuiEnd();

        EndDrawing();

        //SwapScreenBuffer();
    }

    return EXIT_SUCCESS;
}
