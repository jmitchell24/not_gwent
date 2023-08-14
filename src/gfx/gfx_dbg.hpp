#pragma once

#include <ut/math.hpp>
#include <ut/string.hpp>
#include <ut/color.hpp>

#include <vector>



namespace gfx
{
    class DebugDrawManager
    {
    public:
        bool            enabled = false;
        float           scale   = 1;
        ut::vec2f       offset  = {0,0};

        DebugDrawManager();

        void addRect(ut::cstrparam label, ut::rectf const& r);

        bool drawDebug();

    private:
        struct RectDraw
        {
            ut::cstrview    text;
            ut::color       col;
            ut::rectf       bound;
        };

        struct RectTag
        {
            bool            enabled;
            ut::cstrview    text;
            ut::color           col;
            size_t          cnt;

            inline RectDraw toDraw(ut::rectf const& r) const { return { text, col, r}; }
        };

        using tags_type     = std::vector<RectTag>;
        using draws_type    = std::vector<RectDraw>;

        tags_type   m_tags;
        draws_type  m_draws;

        int         m_im_style;
        float       m_im_alpha;

        RectTag& getTag(ut::cstrparam label);
    };
}
