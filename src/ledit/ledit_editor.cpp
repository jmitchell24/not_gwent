//
// Created by james on 10/19/23.
//

#include "ledit/ledit_editor.hpp"
using namespace ledit;

//
// Gfx
//
//#include "gfx/gfx_virt2d.hpp"
//using namespace gfx;

//
// ImGui
//
#include "rlImGui/ed/TextEditor.h"
#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
using namespace ut;

//
// std
//
#include <filesystem>
using namespace std;

//
// Implementation -> BoxEditor
//

BoxEditor::BoxEditor(ut::cstrparam name)
    : m_name{name.str()}
{ }

void BoxEditor::setRoot(rect const& bounds)
{
    root_box        = Box::createRoot(bounds);
    selected_box    = nullptr;
}

rectget_t BoxEditor::getRect(ut::cstrparam s)
{
    if (auto box = getBox(s))
    {
        if (box->getChanged())
        {
            box->setChanged(false);
            return {box->bounds_inner};
        }
    }
    return {};
}

bool BoxEditor::draw()
{
    using namespace ImGui;

    if (!Box::isRoot(root_box))
        return false;

    PushID(m_name.c_str());
    Box::drawOverlay(*this);
    drawMainWindow();
    PopID();

    return true;
}

void BoxEditor::drawMainWindow()
{
    using namespace ImGui;

    auto lbl = PRINTER("Box Editor '%s'###box_hierarchy", m_name.c_str());

    if (Begin(lbl))
    {
        if (BeginTabBar("tabs"))
        {
            if (BeginTabItem("Options"))
            {
                drawMainWindowBoxEditOptions(edit_opts);
                Separator();
                drawMainWindowOverlayOptions(overlay_opts);
                Separator();
                drawMainWindowFileOptions();
                Separator();
                Box::drawBoxHierarchy(*this);
                EndTabItem();
            }

            Box::drawPropertiesTab(*this);
            EndTabBar();
        }
    }
    End();
}

void BoxEditor::drawMainWindowBoxEditOptions(BoxEditOptions &opts)
{
    using namespace ImGui;

    Text("Row Options");

    Columns(3, "row-columns", false);
    Checkbox("add", &opts.show_row_add);
    NextColumn();
    Checkbox("delete", &opts.show_row_delete);
    NextColumn();
    Checkbox("move", &opts.show_row_move);
    NextColumn();
    Checkbox("rename", &opts.show_row_rename);
    NextColumn();
    Checkbox("weight", &opts.show_row_weight);
    NextColumn();
    Checkbox("type", &opts.show_row_type);
    Columns();
}

void BoxEditor::drawMainWindowOverlayOptions(OverlayOptions& opts)
{
    using namespace ImGui;

    Text("View Options");

    if (ButtonDefault("bg_opacity", opts.background.a != OverlayOptions::DEFAULT_BACKGROUND.a))
    { opts.background.a = OverlayOptions::DEFAULT_BACKGROUND.a; }

    if (int o = int( float(opts.background.a) / 255.0f * 100.0f); SliderInt("Opacity (BG)", &o, 0, 100, "%d%%"))
        opts.background.a = b8( float(o) / 100.0f * 255 );

    if (ButtonDefault("background", opts.background != OverlayOptions::DEFAULT_BACKGROUND))
    { opts.background = OverlayOptions::DEFAULT_BACKGROUND; }

    ColorEdit3("Background", opts.background);


    Dummy(GetItemRectSize());

    if (ButtonDefault("bd_opacity", opts.border.a != OverlayOptions::DEFAULT_BORDER.a))
    { opts.border.a = OverlayOptions::DEFAULT_BORDER.a; }

    if (int o = int( float(opts.border.a) / 255.0f * 100.0f); SliderInt("Opacity (BD)", &o, 0, 100, "%d%%"))
        opts.border.a = b8( float(o) / 100.0f * 255 );

    if (ButtonDefault("border", opts.border != OverlayOptions::DEFAULT_BORDER))
    { opts.border = OverlayOptions::DEFAULT_BORDER; }

    ColorEdit3("Border", opts.border);
}

void BoxEditor::drawMainWindowFileOptions()
{
    using namespace ImGui;

    Text("File Options");

    if (m_current_file.empty())
    {
        TextDisabled("No current file");
    }
    else
    {
        TextUnformatted(m_current_file);
    }

    if (Button("Load..."))
    {
        using namespace filesystem;
        path path_layouts{"data/layout"};

        m_layout_files.clear();
        for (auto&& it : directory_iterator{path_layouts})
            m_layout_files.push_back(it.path().string());
        OpenPopup("popup_load_layout");
    }

    if (BeginPopup("popup_load_layout"))
    {
        if (m_layout_files.empty())
        {
            TextDisabled("No Layout Files");
        }
        else
        {
            for (auto&& it : m_layout_files)
            {
                auto it_str = cstrview(it);
                if (Selectable(it_str, it_str == m_current_file))
                    loadFile(it_str);
            }
        }


        EndPopup();
    }

    SameLine();

    if (Button("Save..."))
    {
        using namespace filesystem;
        path path_layouts{"data/layout"};

        m_layout_files.clear();
        for (auto&& it : directory_iterator{path_layouts})
            m_layout_files.push_back(it.path().string());
        memset(m_new_file.data(), '\0', m_new_file.size());
        OpenPopup("popup_save_layout");
    }

    if (BeginPopup("popup_save_layout"))
    {
        for (auto&& it : m_layout_files)
        {
            auto it_str = cstrview(it);
            if (Selectable(it_str))
                saveFile(it_str);
        }

        Separator();

        InputText("###popup_save_layout_text", m_new_file.data(), m_new_file.size());
        SameLine();

        auto new_file_view = cstrview{m_new_file.data()};

        if (ButtonEnabled("Save As", !new_file_view.empty()))
        {
            if (new_file_view.contains('.'))
                saveFile(PRINTER("data/layout/%s", new_file_view.c_str()));
            else
                saveFile(PRINTER("data/layout/%s.yaml", new_file_view.c_str()));

            CloseCurrentPopup();
        }

        EndPopup();
    }
}

void BoxEditor::loadFile(ut::cstrparam filename)
{
    box_ptr new_root = Box::createRoot(root_box->sizer);
    if (new_root->loadYaml(filename))
    {
        m_current_file = filename;
        root_box = new_root;
        selected_box = root_box;
    }
}

void BoxEditor::saveFile(ut::cstrparam filename)
{
    if (root_box->saveYaml(filename))
    {
        m_current_file = filename;
    }
}

