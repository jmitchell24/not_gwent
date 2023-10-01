#pragma once

#include "gfx/gfx_dbg.hpp"

#include <unordered_map>

#include "raylib.h"

#define VIRT_DEBUG(r_)      ( gfx::VIRT.drawRectDebug(#r_, r_) )


#define DRECT2(s_, r_)          do { gfx::VIRT.drawRectDebug(#s_, r_); } while(0)
#define DRECT_PUSH2(s_, r_)     do { gfx::VIRT.pushRectDebug(#s_, r_); } while(0)
#define DRECT1(r_)              do { gfx::VIRT.drawRectDebug(#r_, r_); } while(0)
#define DRECT_PUSH1(r_)         do { gfx::VIRT.pushRectDebug(#r_, r_); } while(0)
#define DRECT_POP()             do { gfx::VIRT.popRectDebug();         } while(0)

namespace gfx
{
    ///
    /// Provides additional drawing functionality, namely a fixed size viewport / screen space for 2d drawing.
    ///
    class Virt2DManager
    {
    public:
        inline ut::rectf realViewport() const
        { return m_real_viewport; }

        inline ut::rectf virtViewport() const
        { return m_virt_viewport; }

        ut::vec2f realPoint(ut::vec2f const &p) const;

        ut::rectf realRect(ut::rectf const &r) const;

        void layout(ut::rectf const &real_viewport, float virt_width, float virt_height);

        ut::color nextDebugColor(size_t seed);

        void begin();

        void end();

        void beginScissor(ut::rectf const &r);

        void endScissor();

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

        static Virt2DManager &instance();

    private:
        ut::rectf m_real_viewport;
        ut::rectf m_virt_viewport;

        float   m_scale                 = 1;
        Matrix  m_transform;

        bool    m_begin_flag            = false;
        bool    m_begin_scissor_flag    = false;

        void pushMatrix();

        void popMatrix();
    };

    [[maybe_unused]] static Virt2DManager &VIRT = Virt2DManager::instance();
}
