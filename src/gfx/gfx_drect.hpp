#pragma once

//
// gfx
//
#include "gfx/gfx_view_transform.hpp"

//
// ut
//
#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>

//
// std
//
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>

#define DRECT2(s_, r_)          do { gfx::DebugRectOverlayManager::instance().addRect (#s_, r_); } while(0)
#define DRECT_PUSH2(s_, r_)     do { gfx::DebugRectOverlayManager::instance().pushRect(#s_, r_); } while(0)
#define DRECT1(r_)              do { gfx::DebugRectOverlayManager::instance().addRect (#r_, r_); } while(0)
#define DRECT_PUSH1(r_)         do { gfx::DebugRectOverlayManager::instance().pushRect(#r_, r_); } while(0)
#define DRECT_POP()             do { gfx::DebugRectOverlayManager::instance().popRect ();        } while(0)

namespace gfx
{
    class DebugRectOverlayManager
    {
    public:
        using vtransform_type = std::optional<gfx::ViewTransform>;

        bool enabled = false;
        vtransform_type view_transform;

        void addRect (ut::cstrparam label, ut::rect const& r);
        void pushRect(ut::cstrparam label, ut::rect const& r);

        void addRect(ut::rect const& r);
        void popRect();

        void drawDebug();

        static DebugRectOverlayManager& instance();

    private:
        struct Overlay
        {
            ut::cstrview    text;
            ut::color       color;
            ut::rect        bound;
            bool            highlighted;
        };

        struct Tag
        {
            using taglist_type = std::vector<Tag>;

            bool                enabled;
            bool                highlighted;
            ut::cstrview        text;
            ut::color           color;
            size_t              overlay_count;
            taglist_type        child_tags;

            inline Overlay toOverlay(ut::rect const& r) const
            {
                return { text, color, r, highlighted };
            }

            void enableAll();
            void disableAll();

            void draw();
            bool draw(bool is_leaf);

            Tag& getChildTag(ut::cstrview const* begin, ut::cstrview const* end);
        };

        using label_type = std::vector<ut::cstrview>;
        using draws_type = std::vector<Overlay>;

        DebugRectOverlayManager();

        Tag         m_root_tag;
        label_type  m_label;
        draws_type  m_draws;
        int         m_im_style;
    };

    static DebugRectOverlayManager& DRECT = DebugRectOverlayManager::instance();
}
