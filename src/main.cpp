/*

    TODO: only apply nudge animation to adjacent cards, not the entire row
    TODO: replace current shadow rendering: 9-patch and a cast shadow
    TODO: implement card rotation
    TODO: lookup how to do vertex arrays in rlgl. Use to build shadow shape.
    TODO: implement ng::Card as the data for game::Card
    TODO: Compound Tween (like spline segments but built out of easings)
*/


//
// ng
//
#include "ng/ng_game_solver.hpp"

//
// game
//
#include "game/conv.hpp"
#include "game/game.hpp"
#include "game/assets.hpp"

using namespace game;

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "gfx/gfx_draw.hpp"
#include "gfx/gfx_curves.hpp"
#include "gfx/gfx_spring.hpp"
#include "gfx/gfx_tween.hpp"
#include "gfx/gfx_spinner.hpp"

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

size_t constexpr static SCREEN_WIDTH = 1920;
size_t constexpr static SCREEN_HEIGHT = 1080;

size_t constexpr static VIRT_WIDTH = 1280;
size_t constexpr static VIRT_HEIGHT = 720;
size_t constexpr static VIRT_PAD = 10;


struct NgDemo
{
    ng::Game game;

    void layout(rect const& bounds)
    {

        game = ng::GameSolver::makeGame();
    }

    void update()
    {

    }

    void draw()
    {

    }

    void draw3d()
    {

    }

    static constexpr cstrview DEBUG_LABEL = "Cards"_sv;


    void drawDebug(cstrparam)
    {

    }

    void drawCards(cstrparam lbl, ng::cardlist_t const& cards)
    {
        auto flags = ImGuiTableFlags_None |
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable(lbl, 10, flags))
        {


            for (auto&& it : cards)
            {
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn())
                {
                    ImGui::TextUnformatted(it.name);
                }
            }




            ImGui::EndTable();
        }


    }
};

#if 0

struct NgDemo
{
    ng::Game game;



    void layout(rect const& bounds)
    {
        m_bounds = bounds.anchorCCtoCC(100,100);
        m_bounds_a = m_bounds;
        m_bounds_b = m_bounds;

        game = ng::GameSolver::makeGame();
    }

    void update()
    {
//        if (!m_tween.update(GetFrameTime()))
//            return;

        m_min = min(m_src, m_dst);
        m_max = max(m_src, m_dst);

        //m_time = m_tween.now();


        float t = m_src > m_dst ? 1.0f - m_time : m_time;
        t *=  (1.0f - FLT_EPSILON);

        m_range         = float(m_max - m_min);
        m_trange        = 1.0f / m_range;
        m_fmod_range    = m_range * abs(fmod(t, m_trange));
        m_i             = int(t * m_range);

        float offset_x = m_bounds.width() * m_fmod_range;

        rect r = m_bounds;

        if (m_src < m_dst)
        {
            m_bounds_a = r.withOffsetX(offset_x);
            m_bounds_b = r.withOffsetX(offset_x - m_bounds.height());

            m_integer_a = m_min + int(m_i);
            m_integer_b = m_integer_a + 1;

            m_color_a = colors::white.withNormalA(std::min(1.0f, std::max(0.0f, 1.0f - m_fmod_range)));
            m_color_b = colors::white.withNormalA(m_fmod_range);
        }
        else
        {
            m_bounds_b = r.withOffsetX(offset_x);
            m_bounds_a = r.withOffsetX(offset_x - m_bounds.height());

            m_integer_b = m_min + int(m_i);
            m_integer_a = m_integer_b + 1;

            m_color_b = colors::white.withNormalA(std::min(1.0f, std::max(0.0f, 1.0f - m_fmod_range)));
            m_color_a = colors::white.withNormalA(std::min(1.0f, std::max(0.0f, m_fmod_range)));
        }
    }

    void draw()
    {


        auto font = fonts::smallburgRegular128();



        VIRT.drawTextCCtoCC(font, m_bounds_a, PRINTER("%d", m_integer_a), m_color_a);
        VIRT.drawTextCCtoCC(font, m_bounds_b, PRINTER("%d", m_integer_b), m_color_b);

        VIRT_DEBUG(m_bounds);
        VIRT_DEBUG(m_bounds_a);
        VIRT_DEBUG(m_bounds_b);
    }

    void draw3d()
    {

    }

    int m_target=10;
    int m_src=0, m_dst=10;
    int m_min=0, m_max=10;

    int   m_i           =0;
    float m_time        =0;
    float m_range       =0;
    float m_trange      =0;
    float m_fmod_range  =0;

    rect m_bounds;
    rect m_bounds_a;
    rect m_bounds_b;

    int m_integer_a;
    int m_integer_b;

    color m_color_a;
    color m_color_b;

    gfx::TweenReal m_tween{easings::elasticOut, 2.0f};

    static constexpr cstrview DEBUG_LABEL = "Cards"_sv;
    void drawDebug(cstrparam)
    {
        ImGui::SliderInt("dst", &m_target, -100, 100);
        ImGui::SliderFloat("t", &m_time, -1,1 );

        if (ImGui::Button("GO"))
        {

            m_src = m_dst;
            m_dst = m_target;
            m_tween.anim(0, 1);
        }



//#define LABEL_(x_) ImGui::LabelText(#x_, "%.2f", x_);
//        LABEL_(m_time);
//        LABEL_(m_range);
//        LABEL_(m_trange);
//        LABEL_(m_fmod_range);
//        LABEL_(m_i);
//#undef LABEL_


//        ImGui::Text("p1 deck (%d)", game.p1.deck.size());
//        drawCards("p1 deck"_sv, game.p1.deck);
//        ImGui::Text("p1 hand (%d)", game.p1.hand.size());
//        drawCards("p1 hand"_sv, game.p1.hand);
    }

    void drawCards(cstrparam lbl, ng::cardlist_t const& cards)
    {
        auto flags = ImGuiTableFlags_None |
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable(lbl, 10, flags))
        {


            for (auto&& it : cards)
            {
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn())
                {
                    ImGui::TextUnformatted(it.name);
                }
            }




            ImGui::EndTable();
        }


    }
};

struct NgDemo
{
    ng::Game game;



    void layout(rect const& bounds)
    {
        m_bounds = bounds.anchorCCtoCC(100,100);
        m_bounds_a = m_bounds;
        m_bounds_b = m_bounds;

        game = ng::GameSolver::makeGame();
    }

    void update()
    {
        if (!m_tween.update(GetFrameTime()))
            return;

        m_min = min(m_src, m_dst);
        m_max = max(m_src, m_dst);

        m_time = m_tween.now();

        float t = m_src > m_dst ? 1.0f - m_time : m_time;
        t *=  (1.0f - FLT_EPSILON);

        m_range         = float(m_max - m_min);
        m_trange        = 1.0f / m_range;
        m_fmod_range    = m_range * fmod(t, m_trange);
        m_i             = int(t * m_range);

        float offset_y = m_bounds.height() * m_fmod_range;

        rect r = m_bounds;

        if (m_src < m_dst)
        {
            m_bounds_a = r.withOffsetY(offset_y);
            m_bounds_b = r.withOffsetY(offset_y - m_bounds.height());

            m_integer_a = m_min + int(m_i);
            m_integer_b = m_integer_a + 1;

            m_color_a = colors::white.withNormalA(1.0f - m_fmod_range);
            m_color_b = colors::white.withNormalA(m_fmod_range);
        }
        else
        {
            m_bounds_b = r.withOffsetY(offset_y);
            m_bounds_a = r.withOffsetY(offset_y - m_bounds.height());

            m_integer_b = m_min + int(m_i);
            m_integer_a = m_integer_b + 1;

            m_color_b = colors::white.withNormalA(1.0f - m_fmod_range);
            m_color_a = colors::white.withNormalA(m_fmod_range);
        }
    }

    void draw()
    {


        auto font = fonts::smallburgRegular128();



        VIRT.drawTextCCtoCC(font, m_bounds_a, PRINTER("%d", m_integer_a), m_color_a);
        VIRT.drawTextCCtoCC(font, m_bounds_b, PRINTER("%d", m_integer_b), m_color_b);

        VIRT_DEBUG(m_bounds);
        VIRT_DEBUG(m_bounds_a);
        VIRT_DEBUG(m_bounds_b);
    }

    void draw3d()
    {

    }

    int m_src=0, m_dst=10;
    int m_min=0, m_max=10;

    int   m_i           =0;
    float m_time        =0;
    float m_range       =0;
    float m_trange      =0;
    float m_fmod_range  =0;

    rect m_bounds;
    rect m_bounds_a;
    rect m_bounds_b;

    int m_integer_a;
    int m_integer_b;

    color m_color_a;
    color m_color_b;

    gfx::TweenReal m_tween{easings::elasticOut, 2.0f};

    static constexpr cstrview DEBUG_LABEL = "Cards"_sv;
    void drawDebug(cstrparam)
    {
        ImGui::SliderInt("dst", &m_dst, -100, 100);
        //ImGui::SliderFloat("t", &m_time, 0,1 );

        if (ImGui::Button("GO"))
        {
            m_src = m_dst;
            m_tween.anim(0, 1);
        }



//#define LABEL_(x_) ImGui::LabelText(#x_, "%.2f", x_);
//        LABEL_(m_time);
//        LABEL_(m_range);
//        LABEL_(m_trange);
//        LABEL_(m_fmod_range);
//        LABEL_(m_i);
//#undef LABEL_


//        ImGui::Text("p1 deck (%d)", game.p1.deck.size());
//        drawCards("p1 deck"_sv, game.p1.deck);
//        ImGui::Text("p1 hand (%d)", game.p1.hand.size());
//        drawCards("p1 hand"_sv, game.p1.hand);
    }

    void drawCards(cstrparam lbl, ng::cardlist_t const& cards)
    {
        auto flags = ImGuiTableFlags_None |
                ImGuiTableFlags_Borders |
                ImGuiTableFlags_RowBg;
        if (ImGui::BeginTable(lbl, 10, flags))
        {


            for (auto&& it : cards)
            {
                ImGui::TableNextRow();
                if (ImGui::TableNextColumn())
                {
                    ImGui::TextUnformatted(it.name);
                }
            }




            ImGui::EndTable();
        }


    }
};

struct CatmullRomDemo
{
    vector<vec2> control_points;
    vec2 *cp_ptr = nullptr;
    rect m_bounds;

    void layout(rect const &bounds)
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

    void drawLineT(float t0, float t1, color const &c)
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
            float t1 = float(i + 1) / float(segments);
            drawLineT(t0, t1, i % 2 == 0 ? col_line : col_line_alt);
        }

        if (show_cc)
        {
            for (size_t i = 0; i < control_points.size(); ++i)
            {
                auto &&it = control_points[i];
                DrawCircleLines(it.x, it.y, 10, torl(col_control_point));

                auto text_it = it + vec2(10);
                DrawText(PRINTER("%d", i + 1).c_str(), text_it.x, text_it.y, 30, torl(ut::colors::white));
            }
        }


    }

    float alpha = 0.5f;
    float tension = 0.0f;
    int segments = 10;
    size_t cc_count = 5;
    bool show_cc = true;

    color col_control_point = colors::orangered;
    color col_line = colors::greenyellow;
    color col_line_alt = colors::darkcyan;

    void drawDebug(cstrparam)
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

    vec2 *getControlPoint(vec2 const &mp)
    {
        for (auto &&it: control_points)
            if (it.distance(mp) < 10)
                return &it;
        return nullptr;
    }
};

struct CardMoverDemo
{


    CardList a{CardLayout::DIR_CENTER, Card::createTestCards(15)};
    CardList b{CardLayout::DIR_CENTER, Card::createTestCards(5)};

    CardMover mover;


    void layout(rect const &bounds)
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

    SpringVec2 spring{0.5f, 3.0f * PI};

    void layout(rect const &bounds)
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

        DrawCircle(now.x, now.y, 10.0f, GREEN);
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
        ImGui::SliderFloat("zeta", &spring.zeta, 0.0, 1.0);

        ImGui::LabelText("Is Done?", "%s", spring.isAtDst() ? "yes" : "no");
    }
};



struct ShadowDemo
{
    constexpr static size_t tex_width = 200;
    constexpr static size_t tex_height = 200;
    constexpr static size_t NUM_TEXTURES = 9;

    Texture2D tex;

    rect m_bounds;

    int m_param_width = 200;
    int m_param_height = 200;
    int m_param_pad = 20;
    int m_param_blur = 20;


    bool m_param_flag = true;

    void layout(rect const &bounds)
    {
        m_bounds = bounds;


    }

    void update()
    {
        if (m_param_flag)
        {
            m_param_flag = false;

            if (IsTextureReady(tex))
                UnloadTexture(tex);

            Image img = GenImageColor(m_param_width, m_param_height, BLANK);

            {
                auto r = rect(psize(m_param_width, m_param_height)).shrunk(m_param_pad);
                ImageDrawRectangleRec(&img, torl(r), BLACK);
                ImageBlurGaussian(&img, m_param_blur);
            }

            tex = LoadTextureFromImage(img);
            UnloadImage(img);
        }

    }

    void draw()
    {
        drawRect(m_bounds, colors::burlywood);
        drawRectOutline(m_bounds, 10, colors::darkslateblue);

        {
            auto r = m_bounds.anchorCCtoCC(m_param_width, m_param_height);


            drawShadow(r, {(float)m_param_pad, (float)m_param_pad}, m_param_blur);

            drawTexture(textures::card_0(), r);

        }

        //drawTexture(tex, m_bounds.anchorCCtoCC(m_param_width,m_param_height));

    }

    void drawDebug(cstrparam label)
    {
        m_param_flag |= ImGui::SliderInt("width"    , &m_param_width, 32, 512);
        m_param_flag |= ImGui::SliderInt("height"   , &m_param_height, 32, 512);
        m_param_flag |= ImGui::SliderInt("pad"      , &m_param_pad, 5, 50);
        m_param_flag |= ImGui::SliderInt("blur"     , &m_param_blur, 5, 50);


    }
};

#endif

struct CardDemo
{
    CardList cards{CardLayout::DIR_CENTER, Card::createTestCards(3)};
    CardMover mover;

    void layout(rect const &bounds)
    {
        cards.layout(bounds.anchorCCtoCC(bounds.size()/2));

        mover.set({&cards});


    }


    void update()
    {
        cards.update();

        mover.update();


    }

    void draw()
    {
        cards.draw();

        mover.draw();


    }

    static constexpr cstrview DEBUG_LABEL = "Card Demo";
    void drawDebug(cstrparam label)
    {

    }
};

// https://github.com/Rabios/awesome-raylib
using game_t = CardDemo;

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

    game_t gg;

    auto window_bounds = rect(0, 0, VIRT_WIDTH, VIRT_HEIGHT).shrunk(VIRT_PAD);
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



        auto real_viewport = ImGui::GetDockspaceViewport();

        VIRT.layout(real_viewport, VIRT_WIDTH, VIRT_HEIGHT);
        VIRT.begin();

        gg.update();
        gg.draw();

        VIRT.end();



        DrawFPS(10, 10);

        rlDrawRenderBatchActive();

        rlImGuiBegin();
        {
            ImGui::RenderDockspace();

            ImGui::Begin("main_window");
            VIRT.drawDebug();
            ImGui::End();

            ImGui::Begin(game_t::DEBUG_LABEL);
            gg.drawDebug(game_t::DEBUG_LABEL);
            ImGui::End();
        }
        rlImGuiEnd();

        EndDrawing();

        //SwapScreenBuffer();
    }

    return EXIT_SUCCESS;
}
