#pragma once

//
// ut
//

#include <memory>
#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>

//
// std
//

#include <vector>
#include <unordered_map>
#include "random.hpp"



namespace gfx
{

    class DebugRectManager
    {
    public:
        bool enabled = false;

        void addRect(ut::cstrparam label, ut::rectf const& r);
        void pushRect(ut::cstrparam label, ut::rectf const& r);

        void addRect(ut::rectf const& r);
        void popRect();

        void drawDebug();

        static DebugRectManager& instance();

    private:
        struct Overlay
        {
            ut::cstrview    text;
            ut::color       color;
            ut::rectf       bound;
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

            inline Overlay toOverlay(ut::rectf const& r) const
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

        DebugRectManager();

        Tag         m_root_tag;
        label_type  m_label;
        draws_type  m_draws;
        int         m_im_style;
    };
}
