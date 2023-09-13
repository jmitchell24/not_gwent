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


        bool            enabled          = false;
        bool            show_rect_labels = true;

        DebugRectManager();

        void addRect(ut::cstrparam label, ut::rectf const& r);
        void pushRect(ut::cstrparam label, ut::rectf const& r);

        void addRect(ut::rectf const& r);
        void popRect();

        bool drawDebug();

    private:
        struct RectDraw
        {
            ut::cstrview    text;
            ut::color       color;
            ut::rectf       bound;
            bool            highlighted;
        };

        struct RectTag
        {
            using taglist_type = std::vector<RectTag>;

            bool                enabled;
            bool                highlighted;
            ut::cstrview        text;
            ut::color           color;
            size_t              count;
            taglist_type        child_tags;

            inline RectDraw toDraw(ut::rectf const& r) const
            {
                return { text, color, r, highlighted };
            }

            void enableAll();
            void disableAll();

            void draw();
            void drawLeaf();
            void drawBranch();

            RectTag& getChildTag(ut::cstrview const* begin, ut::cstrview const* end);
        };

        using label_type = std::vector<ut::cstrview>;
        using draws_type = std::vector<RectDraw>;

        RectTag                 m_root_tag{true, false, "", {}, 0};
        label_type              m_label{""};
        draws_type              m_draws;

        int                     m_im_style;
        float                   m_im_alpha;
    };
}
