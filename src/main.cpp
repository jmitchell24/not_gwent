//
// game
//
#include "game/types.hpp"
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
using namespace std;

#include "raylib.h"
#include "rlgl.h"
#include "rlImGui/rlImGui.h"
#include "rlImGui/imgui/imgui_extra.hpp"
#include "rlImGui/extras/IconsFontAwesome5.h"
//#include <lua.hpp>

size_t constexpr static SCREEN_WIDTH    = 1280;
size_t constexpr static SCREEN_HEIGHT   = 720;

size_t constexpr static VIRT_WIDTH  = 720;
size_t constexpr static VIRT_HEIGHT = 1280;
size_t constexpr static VIRT_PAD    = 10;

void updateCardListInputs(CardList& card_list)
{
    auto mp = tout(GetMousePosition());

    if (card_list.containsPoint(mp))
    {
        card_list.tryHoverCard(mp);
    }
}

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

    CardList card_list(DIR_CENTER,  Card::createTestCards(1));
    //CardList card_list_alt(DIR_CENTER,  Card::createTestCards(4));


    //optional<Card> card_held;


    auto split = window_bounds.splitNV<4>(30);

    card_list.layout(split[1]);

    //card_list_alt.layout(split[2]);

    //Game game;
    //auto bounds = rectf(0, 0, VIRT_WIDTH, VIRT_HEIGHT).shrunk(VIRT_PAD);

    //game.layout(bounds);


//    lua_State* L = luaL_newstate();
//    luaL_openlibs(L); // Open standard Lua libraries
//
//    int result = luaL_dofile(L, "data/scripts/myscript.lua");
//    if (result != 0) {
//        // Handle error
//    }
//
//    lua_close(L);

    size_t ghost = 0;
    vec2 mp;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            if (!card_list.hasGhost())
                card_list.setGhost(ghost);
            else
                card_list.clearGhost();
        }

        if (card_list.hasGhost())
        {

            if (ssize_t idx; card_list.tryGetGhostIndex(mp,idx))
            {
                card_list.setGhost(idx);
            }

//            if (IsKeyPressed(KEY_LEFT))
//            {
//                if (ghost > 0)
//                    card_list.setGhost(--ghost);
//            }
//
//            if (IsKeyPressed(KEY_RIGHT))
//            {
//                if (ghost < card_list.count())
//                    card_list.setGhost(++ghost);
//            }

            if (IsKeyPressed(KEY_F))
            {
                card_list.ghostToCard(Card::createTestCard());
            }
        }

        if (card_list.hasHoveredCard())
        {

        }




        BeginDrawing();

        ClearBackground(BLACK);

        VIRT.layout(ImGui::GetDockspaceViewport(), VIRT_WIDTH, VIRT_HEIGHT);
        VIRT.begin();

        mp = tout(GetMousePosition());

//        if (card_held.has_value())
//        {
//            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
//            {
//                auto mp = tout(GetMousePosition());
//
//                if (card_list.containsPoint(mp))
//                {
//                    card_list.add(0, card_held.value());
//                }
//
//                if (card_list_alt.containsPoint(mp))
//                {
//                    card_list_alt.add(0, card_held.value());
//                }
//
//
//                card_held.reset();
//            }
//        }
//        else
//        {
//            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
//            {
//                auto mp = tout(GetMousePosition());
//                if (Card card; card_list.tryGrabCard(mp, card))
//                    card_held = card;
//
//                if (Card card; card_list_alt.tryGrabCard(mp, card))
//                    card_held = card;
//            }
//        }

        //game.draw();



        updateCardListInputs(card_list);

        card_list.update();
        card_list.draw();

//        card_list_alt.update();
//        card_list_alt.draw();
//
//        if (card_held.has_value())
//        {
//            auto mp = tout(GetMousePosition());
//
//            card_held->targetPosition(mp - card_held->size()/2);
//            card_held->update();
//            card_held->draw();
//        }


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
