#pragma once

#include "gfx/gfx_dbg.hpp"

#include <unordered_map>

#include "raylib.h"

#define VIRT_DEBUG(r_) ( gfx::VIRT.drawRectDebug(#r_, r_) )

namespace gfx
{
    using rect = ut::rectf;
    using vec2 = ut::vec2f;
    using color = ut::color;

    ///
    /// Provides additional drawing functionality, namely a fixed size viewport / screen space for 2d drawing.
    ///
    class Virt2DManager
    {
    public:
        inline rect realViewport() const { return m_real_viewport; }
        inline rect virtViewport() const { return m_virt_viewport; }

        vec2 realPoint(vec2 const& p) const;
        rect realRect(rect const& r) const;

        void layout(rect const& real_viewport, float virt_width, float virt_height);

        color nextDebugColor(size_t seed);

        void begin();
        void end();

        void beginScissor(rect const& r);
        void endScissor();

        //void drawText(rect const& r, float font_size, cstrview const& s, color const& c);

        float   measureTextWidth (ut::cstrparam s, float font_size) const;
        vec2    measureTextSize  (ut::cstrparam s, float font_size) const;
        rect    measureTextRect  (ut::cstrparam s, vec2 const& p, float font_size) const;

        // TODO: these methods calculate a rect for text alignment, consider returning it
#define CASE(x_) \
        void drawText##x_(rect const& r, ut::cstrparam s, color const& c) const; \
        void drawText##x_(rect const& r, float font_size, ut::cstrparam s, color const& c) const;
UT_ENUM_RECT_ALIGNMENTS
#undef REGION_ALIGN

        void drawThinLine(vec2 const& a, vec2 const& b, color const& c);
        void drawRectThinLines(rect const& r, color const& c);

        void drawLine(vec2 const& a, vec2 const& b, float line_width, color const& c) const;
        void drawRectangleLines(rect const& r, float line_width, color const& c) const;

        void drawRectangle(rect const& r, color const& c) const;

        void drawTexture(Texture2D const& texture, rect const& dst) const;
        void drawTexture(Texture2D const& texture, rect const& dst, color const& c) const;
        void drawTexture(Texture2D const& texture, rect const& src, rect const& dst) const;

        //
        // debug
        //

        inline void drawRectDebug(ut::cstrparam lbl, rect const& r)
        { m_debug.addRect(lbl, realRect(r)); }

        inline bool drawDebug()
        { return m_debug.drawDebug(); }

        //
        // Singleton accessor
        //

        static Virt2DManager& instance();

    private:
        rect    m_real_viewport;
        rect    m_virt_viewport;

        float   m_scale=1;
        Matrix  m_transform;

        bool    m_begin_flag    = false;
        bool    m_begin_scissor_flag = false;

        DebugDrawManager m_debug;

        void pushMatrix();
        void popMatrix();
    };

    [[maybe_unused]] static Virt2DManager& VIRT = Virt2DManager::instance();

    //
    // misc
    //

    color GetDebugColor(size_t seed);
}
