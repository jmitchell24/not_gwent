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

size_t constexpr static SCREEN_WIDTH    = 1920;
size_t constexpr static SCREEN_HEIGHT   = 1080;

size_t constexpr static VIRT_WIDTH  = 1280;
size_t constexpr static VIRT_HEIGHT = 720;
size_t constexpr static VIRT_PAD    = 10;

struct Segment
{
    vec2 a,b,c,d;

    float tmin, tmax;

    void layout(vec2 const& p0, vec2 const& p1, vec2 const& p2, vec2 const& p3,
                float alpha, float tension,
                float tmin, float tmax)
    {
        //https://qroph.github.io/2018/07/30/smooth-paths-using-catmull-rom-splines.html
        assert(alpha >= 0 && alpha <= 1);
        assert(tension >= 0 && alpha <= 1);

        auto t01 = pow(p0.distance(p1), alpha);
        auto t12 = pow(p1.distance(p2), alpha);
        auto t23 = pow(p2.distance(p3), alpha);

        auto m1 = ( p2 - p1 + ( ( (p1 - p0) / t01 - (p2 - p0) / (t01 + t12) ) * t12 ) ) * (1.0f - tension);
        auto m2 = ( p2 - p1 + ( ( (p3 - p2) / t23 - (p3 - p1) / (t12 + t23) ) * t12 ) ) * (1.0f - tension);

        a = ( (p1 - p2) *  2.0f ) + m1 + m2;
        b = ( (p1 - p2) * -3.0f ) - m1 - m1 - m2;
        c = m1;
        d = p1;

        this->tmin = tmin;
        this->tmax = tmax;
    }

    static float mapRange(float value, float fromMin, float fromMax, float toMin, float toMax) {
        return toMin + ((value - fromMin) * (toMax - toMin)) / (fromMax - fromMin);
    }

    vec2 point(float _t) const
    {
        float t = mapRange(_t, tmin, tmax, 0, 1);
        return
            ( a * t * t * t ) +
            ( b * t * t ) +
            ( c * t ) +
            ( d );
    }
};

struct Spline
{
    vector<Segment> segments;

    void layout(vector<vec2> const& control_points, float alpha, float tension)
    {
        assert(control_points.size() >= 4);
        assert(alpha >= 0 && alpha <= 1);
        assert(tension >= 0 && tension <= 1);

        size_t sz = control_points.size()-3;
        float td = 1.0f / float(sz);
        float t0 = 0.0f;

        for (size_t i = 0; i < sz; ++i)
        {
            float t1 = t0 + td - FLT_EPSILON;

            Segment s;
            s.layout(
                control_points[i + 0],
                control_points[i + 1],
                control_points[i + 2],
                control_points[i + 3],
                alpha, tension, t0, t1);
            segments.push_back(s);

            t0 = t1;

        }
    }

    vec2 point(float t) const
    {
        assert(t >= 0 && t <= 1);
        auto fi  = (t * (1.0f - FLT_EPSILON) * float(segments.size()));

        auto i = size_t(fi);
        assert(i >= 0 && i < segments.size());

        return segments[i].point(t);
    }
};



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
        auto origin = b.shrunk(10).lc();
        auto increment = vec2(b.width() / float(cc_count), 0);

        control_points.clear();
        for (size_t i = 0; i < cc_count; ++i)
        {
            control_points.emplace_back(origin + increment * float(i));
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

        Spline s;
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
                DrawCircle(it.x, it.y, 10, torl(col_control_point));
                DrawText(PRINTER("%d", i+1).c_str(), it.x, it.y, 30, torl(ut::colors::white));
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
        ImGui::SliderInt("segments", &segments, 1, 30);

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

struct Demo
{


    CardList a{CardList::DIR_CENTER, Card::createTestCards(5)};
    CardList b{CardList::DIR_CENTER, Card::createTestCards(5)};

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
};

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

    CatmullRomDemo demo;
    demo.layout(window_bounds);
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

        ClearBackground(BLACK);

        VIRT.layout(ImGui::GetDockspaceViewport(), VIRT_WIDTH, VIRT_HEIGHT);
        VIRT.begin();


//        gb.update();
//        gb.draw();

        demo.update();
        demo.draw();


        VIRT.end();

        DrawFPS(10,10);

        DrawGrid(10,10);

        rlImGuiBegin();
        {
            ImGui::RenderDockspace();

            ImGui::Begin("main_window");
            {

                demo.drawDebug();
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
