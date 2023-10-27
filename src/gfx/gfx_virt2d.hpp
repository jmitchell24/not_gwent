#pragma once

#include "gfx/gfx_dbg.hpp"
#include "gfx/gfx_view_transform.hpp"

#include <unordered_map>

#include "raylib.h"
#include "rlNanoVG/nanovg.h"

#define VIRT_DEBUG(r_)      ( gfx::VIRT.drawRectDebug(#r_, r_) )


#define DRECT2(s_, r_)          do { gfx::VIRT.drawRectDebug(#s_, r_); } while(0)
#define DRECT_PUSH2(s_, r_)     do { gfx::VIRT.pushRectDebug(#s_, r_); } while(0)
#define DRECT1(r_)              do { gfx::VIRT.drawRectDebug(#r_, r_); } while(0)
#define DRECT_PUSH1(r_)         do { gfx::VIRT.pushRectDebug(#r_, r_); } while(0)
#define DRECT_POP()             do { gfx::VIRT.popRectDebug();         } while(0)

#define flt_        float
#define vec2_       ut::vec2f const&
#define rect_       ut::rectf const&
#define color_      ut::color const&
#define text_       ut::cstrparam
#define quad_       Quad const&
#define vert_       Vertex const&

namespace gfx
{
    ///
    /// Provides additional drawing functionality, namely a fixed size viewport / screen space for 2d drawing.
    ///
    class Virt2DManager
    {
    public:
        ViewTransform view_transform;

        inline ut::mat4 transformMatrix() const { return m_transform_matrix; }
        inline ut::rectf realViewport() const { return m_real_viewport; }
        //inline ut::rectf virtViewport() const { return m_virt_viewport; }

        ut::vec2f realPoint(vec2_ p) const;
        ut::rectf realRect (rect_ r) const;

        ut::vec2f virtPoint(vec2_ p) const;
        ut::rectf virtRect (rect_ r) const;

        void layout(ut::rectf const &real_viewport, float virt_width, float virt_height);

        void begin();

        void end();

        //void drawText(rect const& r, float font_size, cstrview const& s, color const& c);

        float measureTextWidth(ut::cstrparam s, float font_size) const;

        ut::vec2f measureTextSize(ut::cstrparam s, float font_size) const;

        ut::rectf measureTextRect(ut::cstrparam s, ut::vec2f const &p, float font_size) const;

        // TODO: these methods calculate a rect for text alignment, consider returning it
#define CASE(x_) \
        void drawText##x_(ut::rectf const& r, ut::cstrparam s, ut::color const& c) const; \
        void drawText##x_(ut::rectf const& r, float h, ut::cstrparam s, ut::color const& c) const; \
        void drawText##x_(Font font, ut::rectf const& r, ut::cstrparam s, ut::color const& c) const; \
        void drawText##x_(Font font, ut::rectf const& r, float h, ut::cstrparam s, ut::color const& c) const;

        UT_ENUM_RECT_ALIGNMENTS
#undef CASE

        void drawThinLine(ut::vec2f const &a, ut::vec2f const &b, ut::color const &c);

        void drawRectThinLines(ut::rectf const &r, ut::color const &c);

        void drawLine(ut::vec2f const &a, ut::vec2f const &b, float line_width, ut::color const &c) const;

        void drawRectangleLines(ut::rectf const &r, float line_width, ut::color const &c) const;

        void drawRectangle(ut::rectf const &r, ut::color const &c) const;

        void drawTextureV(Texture2D texture, ut::vec2f const &position,
                          ut::color const &c);                                                       // Draw a Texture2D with position defined as Vector2
        void drawTextureEx(Texture2D texture, ut::vec2f const &position, float rotation, float scale,
                           ut::color const &c);                          // Draw a Texture2D with extended parameters

        void drawTextureRec(Texture2D texture, ut::vec2f const &position, ut::color const &c);

        void drawTextureRec(Texture2D texture, ut::rectf const &source, ut::vec2f const &position,
                            ut::color const &c);                                   // Draw a part of a texture defined by a rectangle

        void drawTexturePro(Texture2D texture, ut::rectf const &dest, ut::color const &c);

        void drawTexturePro(Texture2D texture, ut::rectf const &source, ut::rectf const &dest, ut::vec2f const &origin,
                            float rotation,
                            ut::color const &c);   // Draw a part of a texture defined by a rectangle with 'pro' parameters


        //
        // debug
        //

        inline void drawRectDebug(ut::cstrparam lbl, ut::rectf const &r) const
        { DebugRectManager::instance().addRect(lbl, realRect(r)); }

        inline void pushRectDebug(ut::cstrparam lbl, ut::rectf const &r) const
        { DebugRectManager::instance().pushRect(lbl, realRect(r)); }

        inline void popRectDebug() const
        { DebugRectManager::instance().popRect(); }

        inline void drawDebug() const
        { DebugRectManager::instance().drawDebug(); }

        inline bool isRectDebugEnabled() const
        { return DebugRectManager::instance().enabled; }

        //
        // Singleton accessor
        //

        static Virt2DManager& instance();

    private:
        ut::rectf m_real_viewport;
        //ut::rectf m_virt_viewport;

        float   m_scale                 = 1;
        ut::mat4 m_transform_matrix;
        ut::mat4 m_transform_matrix_inverse;

        bool    m_begin_flag            = false;
        bool    m_begin_scissor_flag    = false;

        void pushMatrix();

        void popMatrix();
    };

    [[maybe_unused]] static Virt2DManager &VIRT = Virt2DManager::instance();
}

#undef flt_
#undef vec2_
#undef rect_
#undef color_
#undef text_
#undef quad_
#undef vert_