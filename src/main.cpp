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
#include "gfx/gfx_draw.hpp"
#include "gfx/gfx_curves.hpp"
#include "gfx/gfx_spring.hpp"
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

size_t constexpr static SCREEN_WIDTH    = 1920;
size_t constexpr static SCREEN_HEIGHT   = 1080;

size_t constexpr static VIRT_WIDTH  = 1280;
size_t constexpr static VIRT_HEIGHT = 720;
size_t constexpr static VIRT_PAD    = 10;


struct CatmullRomDemo
{
    vector<vec2> control_points;
    vec2* cp_ptr=nullptr;
    rect m_bounds;

    void layout(rect const& bounds)
    {
        m_bounds = bounds;
        layoutControlPoints();
    }

    void layoutControlPoints()
    {

        auto b = m_bounds.shrunk(100);
        auto org = b.shrunk(10).lc();
        auto inc = vec2(b.width() / float(cc_count), 0);

        control_points.clear();
        for (size_t i = 0; i < cc_count; ++i)
        {
            control_points.emplace_back(org + inc * float(i));
        }
    }

    void update()
    {
        auto mp = tout(GetMousePosition());

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            cp_ptr = getControlPoint(mp);
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            cp_ptr = nullptr;
        }

        if (cp_ptr != nullptr)
        {
            *cp_ptr = mp;
        }
    }

    void drawLineT(float t0, float t1, color const& c)
    {
//        Segment s;
//        s.layout(control_points[0], control_points[1], control_points[2], control_points[3], alpha, tension);

        CatmullRomSpline s;
        s.layout(control_points, alpha, tension);

        auto p0 = s.point(t0);
        auto p1 = s.point(t1);

        DrawLineEx(torl(p0), torl(p1), 5.0f, torl(c));
    }

    void draw()
    {
        size_t sz = size_t(segments);

        for (size_t i = 0; i < segments; ++i)
        {
            float t0 = float(i) / float(segments);
            float t1 = float(i+1) / float(segments);
            drawLineT(t0, t1, i % 2 == 0 ? col_line : col_line_alt);
        }

        if (show_cc)
        {
            for (size_t i = 0; i < control_points.size(); ++i)
            {
                auto&& it = control_points[i];
                DrawCircleLines(it.x, it.y, 10, torl(col_control_point));

                auto text_it = it + vec2(10);
                DrawText(PRINTER("%d", i+1).c_str(), text_it.x, text_it.y, 30, torl(ut::colors::white));
            }
        }


    }

    float   alpha       =   0.5f;
    float   tension     =   0.0f;
    int     segments    =   10;
    size_t  cc_count    =   5;
    bool    show_cc     =   true;

    color col_control_point = colors::orangered;
    color col_line          = colors::greenyellow;
    color col_line_alt      = colors::darkcyan;

    void drawDebug()
    {
        ImGui::Begin("CatmullRomDemo");

        ImGui::Checkbox("show cc", &show_cc);

        if (int x = cc_count; ImGui::SliderInt("cc count", &x, 4, 10))
        {
            if (x != cc_count)
            {
                cc_count = x;
                layoutControlPoints();

            }
        }

        ImGui::SliderFloat("alpha", &alpha, 0.0f, 1.0f);
        ImGui::SliderFloat("tension", &tension, 0.0f, 1.0f);
        ImGui::SliderInt("segments", &segments, 1, 200);

#define COLOR_EDIT(x_) \
    do \
    { \
        if (vec3 v = x_.toNormal().toVec3(); ImGui::ColorEdit3(#x_, v.pack)) \
            x_ = color::normal(v).toColor(); \
    } while(false)

        COLOR_EDIT(col_control_point);
        COLOR_EDIT(col_line);
        COLOR_EDIT(col_line_alt);

        ImGui::End();

    }

    vec2* getControlPoint(vec2 const& mp)
    {
        for (auto&& it: control_points)
            if (it.distance(mp) < 10)
                return &it;
        return nullptr;
    }
};

struct CardMoverDemo
{
//TODO: pick up card with offset, instead of centering
//TODO: only apply nudge animation to adjacent cards, not the entire row
//TODO: replace current shadow rendering: 9-patch and a cast shadow
//TODO: fix card render ordering issues

    CardList a{CardLayout::DIR_CENTER, Card::createTestCards(15)};
    CardList b{CardLayout::DIR_CENTER, Card::createTestCards(5)};

    CardMover mover;



    void layout(rect const& bounds)
    {
        a.layout(bounds.col(4, 1, {.inner_pad=10, .outer_pad=10}));
        b.layout(bounds.col(4, 2, {.inner_pad=10, .outer_pad=10}));

        mover.set({&a, &b});


    }



    void update()
    {
        a.update();
        b.update();

        mover.update();


    }

    void draw()
    {
        a.draw();
        b.draw();

        mover.draw();




    }

    void drawDebug(cstrparam label)
    {

    }
};

struct ProcAnimDemo
{
    bool drag = false;

    SpringVec2 spring { 0.5f, 3.0f * PI };

    void layout(rect const& bounds)
    {

        spring.setVel({});
        spring.setNow(bounds.center());
        spring.setDst(bounds.center());

    }

    void update()
    {
        auto mp = tout(GetMousePosition());

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mp.distance(spring.dst()) < 10.0)
        {
            drag = true;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            drag = false;
        }

        if (drag)
        {
            spring.setDst(mp);
        }

        auto t = GetFrameTime();

        spring.update(t);
    }

    void draw()
    {
        auto now = spring.now();
        auto dst = spring.dst();

        DrawCircle     (now.x, now.y, 10.0f, GREEN);
        DrawCircleLines(dst.x, dst.y, 10.0f, RED);

    }

    void drawDebug(cstrparam label)
    {
        auto now = spring.now();
        auto vel = spring.vel();
        auto dst = spring.dst();

        ImGui::LabelText("now", "(%.2f,%.2f)", now.x, now.y);
        ImGui::LabelText("vel", "(%.2f,%.2f)", vel.x, vel.y);
        ImGui::LabelText("dst", "(%.2f,%.2f)", dst.x, dst.y);

        ImGui::SliderFloat("omega", &spring.omega, 0.0, 10 * PI);
        ImGui::SliderFloat("zeta" , &spring.zeta , 0.0, 1.0);

        ImGui::LabelText("Is Done?", "%s", spring.isAtDst() ? "yes": "no");
    }
};

struct CardDemo
{
    Card card;

    void layout(rect const& bounds)
    {

        card = Card::createTestCard();
        card.layout(bounds);


    }
    float t = 0.0f;
    void update()
    {
        card.update();
    }

    void draw()
    {
        card.draw();



    }

    void debugDraw(cstrparam label)
    {

    }
};





using game_t = CardMoverDemo;
int main()
{
    SetTargetFPS(120);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "not gwent");

    rlImGuiBeginInitImGui();
    rlImGuiAddFontAwesomeIconFonts(9);
    rlImGuiReloadFonts();
    rlImGuiEndInitImGui();


    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    auto window_bounds = rect(0, 0, VIRT_WIDTH, VIRT_HEIGHT).shrunk(VIRT_PAD);


    game_t gg;

    gg.layout(window_bounds);

//    GameBoard gb;
//    gb.layout(window_bounds);


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

        ClearBackground(GRAY);

        VIRT.layout(ImGui::GetDockspaceViewport(), VIRT_WIDTH, VIRT_HEIGHT);
        VIRT.begin();


        gg.update();
        gg.draw();


        VIRT.end();

        DrawFPS(10,10);

        DrawGrid(10,10);

        rlImGuiBegin();
        {
            ImGui::RenderDockspace();

            ImGui::Begin("main_window");
            VIRT.drawDebug();
            ImGui::End();

            ImGui::Begin("gg");
            gg.drawDebug("gg"_sv);
            ImGui::End();
        }
        rlImGuiEnd();

        EndDrawing();

        //SwapScreenBuffer();
    }

    return EXIT_SUCCESS;
}
