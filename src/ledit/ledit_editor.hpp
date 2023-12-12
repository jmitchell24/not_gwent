//
// Created by james on 10/19/23.
//

#pragma once

#include "ledit/ledit_box.hpp"
#include "ledit/ledit_options.hpp"

namespace ledit
{
    class BoxEditor : private BoxVisitor
    {
    public:
        using BoxVisitor::view_transform;

        BoxEditor(ut::cstrparam name);

        inline bool isPropertiesWindowOpen() const { return is_properties_window_open; }
        inline bool isOverlayVisible() const { return is_overlay_visible; }
        inline bool wantCaptureMouse() const { return want_capture_mouse; }

        void setRoot(ut::rect const& bounds);
        void setFile(ut::cstrparam file);
        bool tryGetRects (ut::cstrparam name, BoxRects& rects);
        bool tryGetOuter (ut::cstrparam name, ut::rect& outer);
        bool tryGetBorder(ut::cstrparam name, ut::rect& border);
        bool tryGetInner (ut::cstrparam name, ut::rect& inner);

        bool draw();
        void drawMenu();

    private:
        using filelist_type = std::vector<std::string>;
        using filetext_type = std::array<char, 30>;

        filetext_type       m_new_file;
        std::string         m_current_file;
        filelist_type       m_layout_files;
        bool                m_want_active = false;
        box_ptr             m_root_box_revert;

        void drawMainWindow();

        void drawMainWindowOptions();
        void drawMainWindowBoxEditOptions(BoxEditOptions& opts);
        void drawMainWindowOverlayOptions(OverlayOptions& opts);
        void drawMainWindowFileOptions();
        void drawMainWindowBindOptions();

        void loadFile(ut::cstrparam filename);
        void saveFile(ut::cstrparam filename);


    };
}