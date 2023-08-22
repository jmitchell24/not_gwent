#include "gfx/gfx_virt2d.hpp"
#include "random.hpp"
#include "rlImGui/imgui/imgui_extra.hpp"
using namespace gfx;

#include "game/conv.hpp"
using namespace game;

#include <raymath.h>
#include <raylib.h>
#include <rlgl.h>

//
// ut
//
#include <ut/random.hpp>
using namespace ut;

#include <vector>
#include <iostream>
using namespace std;



//
// Virt2DManager
//

#define ASSERT_BEGIN_FLAG assert(m_begin_flag && "Virtual screen-space is not active")

Virt2DManager& Virt2DManager::instance()
{
    static Virt2DManager v2d;
    return v2d;
}

vec2 Virt2DManager::realPoint(vec2 const& p) const
{
    auto& mat = m_transform;

    if (m_debug.enabled)
    {
        float x = p.x * m_debug.scale + m_debug.offset.x;
        float y = p.y * m_debug.scale + m_debug.offset.y;
        float z = 0;

        return
        {
            mat.m0*x + mat.m4*y + mat.m8*z + mat.m12,
            mat.m1*x + mat.m5*y + mat.m9*z + mat.m13
        };
    }
    else
    {
        float x = p.x;
        float y = p.y;
        float z = 0;

        return
        {
            mat.m0*x + mat.m4*y + mat.m8*z + mat.m12,
            mat.m1*x + mat.m5*y + mat.m9*z + mat.m13
        };
    }
}

rectf Virt2DManager::realRect(rectf const& r) const
{
    return { realPoint(r.min), realPoint(r.max) };
}

//Virt2DManager::vec2 Virt2DManager::measureText(char const* text, unsigned font_size) const
//{
//    return
//    {
//        MeasureText(text, (int)font_size) * m_scale,
//        font_size * m_scale
//    };
//}

float Virt2DManager::measureTextWidth(cstrparam s, float font_size) const
{
    return (float)MeasureText(s.c_str(), (int)font_size);
}

vec2f Virt2DManager::measureTextSize(cstrparam s, float font_size) const
{
    return { (float)MeasureText(s.c_str(), (int)font_size), font_size };
}

rectf Virt2DManager::measureTextRect(cstrparam s, vec2f const& p, float font_size) const
{
    auto sz = measureTextSize(s, font_size);
    return { p, p+sz };
}

void Virt2DManager::layout(rectf const& real_viewport, float virt_width, float virt_height)
{
    auto [fit_sz, fit_scale] = real_viewport.fitScale(virt_width, virt_height);
    auto fit_rect = real_viewport.anchorCCtoCC(fit_sz);

    auto mx_scale = MatrixScale(fit_scale,fit_scale, 1.0);
    auto mx_trans = MatrixTranslate(fit_rect.x(), fit_rect.y(), 0.0);

    m_transform = MatrixMultiply(mx_scale, mx_trans);

    m_scale     = max(0.0001f, fit_scale);

    m_real_viewport.set(fit_rect);
    m_virt_viewport.set(0,0,virt_width,virt_height);
}

void Virt2DManager::begin()
{
    assert(!m_begin_flag && "Virtual screen-space is already active");
    m_begin_flag = true;

    // apply to matrix stack
    pushMatrix();

    // apply to mouse
    SetMouseOffset(-m_real_viewport.x(), -m_real_viewport.y());
    SetMouseScale(1.0/m_scale, 1.0/m_scale);
}

void Virt2DManager::end()
{
    assert(m_begin_flag && "Virtual screen-space is not active");
    m_begin_flag = false;

    // reset matrix stack
    rlPopMatrix();

    // reset mouse
    SetMouseOffset(0,0);
    SetMouseScale(1,1);
}

void Virt2DManager::beginScissor(rectf const& virt_scissor_bound)
{
    assert(m_begin_flag && "Virtual screen-space is not active");
    assert(!m_begin_scissor_flag && "Virtual scissor is already active");
    VIRT_DEBUG(virt_scissor_bound);

    auto tmp    = realRect(virt_scissor_bound);
    auto sx     = tmp.left;
    auto sy     = GetScreenHeight() - tmp.bottom;
    auto sw     = tmp.width();
    auto sh     = tmp.height();

    rlDrawRenderBatchActive();
    rlEnableScissorTest();
    rlScissor(sx,sy,sw,sh);

    m_begin_scissor_flag = true;
}

void Virt2DManager::endScissor()
{
    assert(m_begin_flag && "Virtual screen-space is not active");
    assert(m_begin_scissor_flag && "Virtual scissor is not active");

    rlDrawRenderBatchActive();
    rlDisableScissorTest();

    m_begin_scissor_flag = false;
}

#define CASE(x_) \
void Virt2DManager::drawText##x_(rectf const& r, float font_size, cstrparam s, color const& c) const \
{ \
    assert(font_size > 0); \
    auto w      = MeasureText(s.c_str(), font_size); \
    auto h      = font_size; \
    auto r_text = r.anchor##x_(w,h); \
    \
    DrawText(s.c_str(), r_text.min.x, r_text.min.y, font_size, torl(c)); \
    \
} \
void Virt2DManager::drawText##x_(rectf const& r, cstrparam s, color const& c) const \
{ drawText##x_(r, r.height(), s, c); }

    UT_ENUM_RECT_ALIGNMENTS
#undef CASE

void Virt2DManager::pushMatrix()
{
    rlPushMatrix();
    rlMultMatrixf(MatrixToFloat(m_transform));

    if (m_debug.enabled)
    {
        rlTranslatef(m_debug.offset.x, m_debug.offset.y, 0);
        rlScalef(m_debug.scale, m_debug.scale, 1);
    }
}

void Virt2DManager::popMatrix()
{
    rlPopMatrix();
}

void Virt2DManager::drawThinLine(vec2 const& a, vec2 const& b, color const& c)
{
    ASSERT_BEGIN_FLAG;

    popMatrix();

    auto real_a = torl(realPoint(a));
    auto real_b = torl(realPoint(b));

    DrawLineEx(real_a, real_b, 1.0f, torl(c));

    pushMatrix();
}

void Virt2DManager::drawRectThinLines(rectf const& r, color const& c)
{
    ASSERT_BEGIN_FLAG;

    popMatrix();

    auto real_rect = torl(realRect(r));

    DrawRectangleLinesEx(real_rect, 1.0f, torl(c));

    pushMatrix();
}

void Virt2DManager::drawLine(vec2 const& a, vec2 const& b, float line_width, color const& c) const
{
    ASSERT_BEGIN_FLAG;
    DrawLineEx(torl(a), torl(b), line_width, torl(c));
}

void Virt2DManager::drawRectangleLines(rectf const& r, float line_width, color const& c) const
{
    ASSERT_BEGIN_FLAG;
    DrawRectangleLinesEx(torl(r), line_width, torl(c));
}

void Virt2DManager::drawRectangle(rectf const& r, color const& c) const
{
    ASSERT_BEGIN_FLAG;
    DrawRectanglePro(torl(r), {0,0}, 0, torl(c));
}

struct line_of_verts
{

    struct tris { vec2 a,b,c,d; };
    std::vector<tris> strips;

    void layout(std::vector<vec2> const& points, float width)
    {
        if (points.size() <= 2)
            return;

        strips.clear();
        for (size_t i = 2; i < points.size(); i+=2)
        {
            auto p1 = points[i-2];
            auto p2 = points[i-1];
            auto p3 = points[i];

            auto n1 = (p2 - p1).normal();
            auto n2 = (p3 - p2).normal();

            auto tan = (n2 - n1).normal();

            auto w2 = width / 2.0f;

            auto v1 = p1 + tan * w2;
            auto v2 = p1 - tan * w2;

            auto v3 = p2 + tan * w2;
            auto v4 = p2 - tan * w2;

            auto v5 = p3 + tan * w2;
            auto v6 = p3 - tan * w2;

            strips.push_back({v1,v2,v3,v4});
            strips.push_back({v3,v4,v5,v6});
        }
    }

    void draw(Color color) const
    {
        rlBegin(RL_TRIANGLES);
        rlColor4ub(color.r, color.g, color.b, color.a/2);
        for (auto&& it: strips)
        {
            rlVertex2f(it.c.x, it.c.y);
            rlVertex2f(it.b.x, it.b.y);
            rlVertex2f(it.a.x, it.a.y);

            rlVertex2f(it.b.x, it.b.y);
            rlVertex2f(it.c.x, it.c.y);
            rlVertex2f(it.d.x, it.d.y);
        }
        rlEnd();
    }

};

//void Virt2DManager::drawTexture(Texture2D const& texture, rect const& src, rect const& dst) const
//{
//    ASSERT_BEGIN_FLAG;
//    assert(IsTextureReady(texture));
//
//    DrawTexturePro(texture, torl(src), torl(dst), Vector2(), 0.0f, WHITE);
//}
//
//
//void Virt2DManager::drawTexture(Texture2D const& texture, rectf const& dst) const
//{
//    ASSERT_BEGIN_FLAG;
//    assert(IsTextureReady(texture));
//
//    auto src = Rectangle{0.0f,0.0f,(float)texture.width,(float)texture.height};
//
//    DrawTexturePro(texture, src, torl(dst), {0,0}, 0, WHITE);
//}
//
//void Virt2DManager::drawTexture(Texture2D const& texture, rect const& dst, color const& c) const
//{
//    ASSERT_BEGIN_FLAG;
//    assert(IsTextureReady(texture));
//
//    auto src = Rectangle{0.0f,0.0f,(float)texture.width,(float)texture.height};
//
//    auto org = dst.size()/2;
//
//
//    DrawTexturePro(texture, src, torl(dst.withOffset(org)), torl(org), 45, torl(c));
//}

void Virt2DManager::drawTextureV(Texture2D texture, vec2 const& position, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));
    DrawTextureV(texture, torl(position), torl(c));
}

void Virt2DManager::drawTextureEx(Texture2D texture, vec2 const& position, float rotation, float scale, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));
    DrawTextureEx(texture, torl(position), rotation, scale, torl(c));
}

void Virt2DManager::drawTextureRec(Texture2D texture, rect const& source, vec2 const& position, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));
    DrawTextureRec(texture, torl(source), torl(position), torl(c));
}

void Virt2DManager::drawTextureRec(Texture2D texture, vec2 const& position, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));
    DrawTextureRec(texture, torl(psize((float)texture.width, (float)texture.height)), torl(position), torl(c));
}

void Virt2DManager::drawTexturePro(Texture2D texture, rect const& dest, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));

    auto source   = psizef((float)texture.width, (float)texture.height);
    auto origin   = vec2();
    auto rotation = 0.0f;

    DrawTexturePro(texture, torl(source), torl(dest), torl(origin), rotation, torl(c));
}

void Virt2DManager::drawTexturePro(Texture2D texture, rect const& source, rect const& dest, vec2 const& origin, float rotation, color const& c)
{
    ASSERT_BEGIN_FLAG; assert(IsTextureReady(texture));
    DrawTexturePro(texture, torl(source), torl(dest), torl(origin), rotation, torl(c));
}
