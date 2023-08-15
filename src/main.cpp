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

class CardHoldDemo
{
public:

private:
};

class CardListDemo
{
public:
    CardListDemo() :
        m_cl1(DIR_CENTER, Card::createTestCards(3)),
        m_cl2(DIR_CENTER, Card::createTestCards(3)),
        m_cl3(DIR_CENTER, Card::createTestCards(3))
    {}

    void layout(rect const& bounds)
    {
        auto split = bounds.splitNV<5>(VIRT_PAD);

        m_cl1.layout(split[1]);
        m_cl2.layout(split[2]);
        m_cl3.layout(split[3]);
    }

    void update()
    {
        auto mp = tout(GetMousePosition());

        updateCL(mp, m_cl1);
        updateCL(mp, m_cl2);
        updateCL(mp, m_cl3);

        m_cl1.update();
        m_cl2.update();
        m_cl3.update();

        if (m_held)
        {
            auto sz2 = m_held->card.size()/2;
            m_held->card.targetPosition(mp-sz2);
            m_held->card.update();
        }
    }

    void draw()
    {
        m_cl1.draw();
        m_cl2.draw();
        m_cl3.draw();

        if (m_held)
            m_held->card.draw();
    }
private:
    struct Held
    {
        Card card;
    };


    CardList m_cl1, m_cl2, m_cl3;

    optional<Held> m_held;


    void updateCL(vec2 const& mp, CardList& cl)
    {
        if (m_held)
        {
            if (size_t idx; cl.tryGetGhostIndex(mp, idx))
            {


                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    cl.addCard(idx, m_held->card, CardList::ANIM_MOVE);
                    m_held.reset();
                }
                else
                {
                    cl.setGhost(idx);
                }
            }
            else
            {
                cl.clearGhost();
            }
        }
        else
        {
            if (size_t idx; cl.tryGetHoverIndex(mp, idx))
            {


                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    m_held = { cl.removeCard(idx)};
                    m_held->card.targetElevation(2.0f);
                    m_held->card.targetOpacity(0.5f);
                    cl.setGhost(idx);
                }
                else
                {
                    cl.setHover(idx);
                }
            }
            else
            {
                cl.clearHover();
            }
        }
    }
};

//void updateCardListInputs(CardList& card_list)
//{
//    auto mp = tout(GetMousePosition());
//
//    if (size_t idx; card_list.tryGetHoverIndex(mp, idx))
//    {
//        card_list.setHover(idx);
//    }
//    else
//    {
//        card_list.clearHover();
//    }
//}

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

    CardListDemo cld;

    cld.layout(window_bounds);


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



        cld.update();
        cld.draw();


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
