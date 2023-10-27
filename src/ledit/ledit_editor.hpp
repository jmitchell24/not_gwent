//
// Created by james on 10/19/23.
//

#pragma once

#include "ledit/ledit_box.hpp"

namespace ledit
{
    class BoxEditor : private BoxVisitor
    {
    public:
        using BoxVisitor::view_transform;

        BoxEditor(ut::cstrparam name);

        void setRoot(ut::rect const& bounds);
        rectget_t getRect(ut::cstrparam name);

        bool draw();

    private:


        using filelist_type = std::vector<std::string>;
        using filetext_type = std::array<char, 30>;

        std::string         m_name;
        filetext_type       m_new_file;
        std::string         m_current_file;
        filelist_type       m_layout_files;
        box_ptr             m_root_box;
        bool                m_properties_window_open;

        void drawOverlay();
        void drawMainWindow();

        void drawMainWindowBoxEditOptions(BoxEditOptions& opts);
        void drawMainWindowOverlayOptions(OverlayOptions& opts);
        void drawMainWindowFileOptions();
        void drawMainWindowBoxHierarchy();

        void loadFile(ut::cstrparam filename);
        void saveFile(ut::cstrparam filename);
    };
}