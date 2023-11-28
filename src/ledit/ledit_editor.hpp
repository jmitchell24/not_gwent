//
// Created by james on 10/19/23.
//

#pragma once

#include "ledit/ledit_box.hpp"

#include "rlImGui/ed/TextEditor.h"

namespace ledit
{
    class BoxEditor : private BoxVisitor
    {
    public:
        using BoxVisitor::view_transform;

        BoxEditor(ut::cstrparam name);

        inline bool wantCaptureMouse() const
        { return overlay_opts.want_capture_mouse; }

        void setRoot(ut::rect const& bounds);
        bool tryGetRects (ut::cstrparam name, BoxRects& rects);
        bool tryGetOuter (ut::cstrparam name, ut::rect& outer);
        bool tryGetBorder(ut::cstrparam name, ut::rect& border);
        bool tryGetInner (ut::cstrparam name, ut::rect& inner);

        bool draw();

    private:
        using filelist_type = std::vector<std::string>;
        using filetext_type = std::array<char, 30>;

        filetext_type       m_new_file;
        std::string         m_current_file;
        TextEditor          m_source_code;
        filelist_type       m_layout_files;


        bool m_is_code_window_open = false;

        void drawMainWindow();
        void drawCodeWindow();

        void drawMainWindowBoxEditOptions(BoxEditOptions& opts);
        void drawMainWindowOverlayOptions(OverlayOptions& opts);
        void drawMainWindowFileOptions();
        void drawMainWindowBindOptions();

        void loadFile(ut::cstrparam filename);
        void saveFile(ut::cstrparam filename);
    };
}