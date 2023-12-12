//
// Created by james on 12/6/23.
//

#pragma once

#include "ledit/ledit_types.hpp"

namespace ledit
{
    struct BoxEditOptions
    {

        bool show_row_add       = false;
        bool show_row_delete    = false;
        bool show_row_move      = false;
        bool show_row_rename    = false;
        bool show_row_weight    = false;
        bool show_row_type      = false;
    };

    struct OverlayOptions
    {
        struct Style { ut::color background, border; };

        static constexpr Style STYLE_PRESET_DEFAULT {  {15,15,15,240}, {110,110,128,128} };
        static constexpr Style STYLE_PRESET_OPAQUE  {  {15,15,15,255}, {110,110,128,128} };
        static constexpr Style STYLE_PRESET_FADE    {  {15,15,15,64 }, {255,255,255,255} };
        static constexpr Style STYLE_PRESET_HIDE    {  { 0, 0, 0,  0}, {  0,  0,  0,  0} };

        std::array<Style, 4> styles
        {
            STYLE_PRESET_DEFAULT,
            STYLE_PRESET_OPAQUE,
            STYLE_PRESET_FADE,
            STYLE_PRESET_HIDE
        };

        size_t style_index = 2;


        inline Style& style()
        {
            return styles[style_index%styles.size()];
        }
    };

    class Options
    {
    public:
        std::string active_editor_name;

        BoxEditOptions box_edit_options;
        OverlayOptions overlay_options;

        static Options& instance();

        inline bool hasActive()
        { return !active_editor_name.empty(); }

        inline void clearActive()
        { active_editor_name.clear(); }

        void showDebugWindow();

        void drawDebug();
        void drawDebugMenu();

    private:
        bool m_show_options_window=false;

        explicit Options();

        static bool drawBoxEditOptions(BoxEditOptions &opts);
        static bool drawOverlayOptions(OverlayOptions &opts);

        void loadConfig();
        void saveConfig() const;
    };

    static Options& LEDIT_OPTIONS = Options::instance();
}