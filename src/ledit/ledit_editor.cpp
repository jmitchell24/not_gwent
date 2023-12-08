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
#include "rlImGui/extras/lucide_icons.hpp"
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

BoxEditor::BoxEditor(cstrparam name)
    : BoxVisitor{name.str()}
{}

void BoxEditor::setRoot(rect const& bounds)
{
    root_box = Box::createRoot(bounds);

    clearSelectedBoxSingle();
}

bool BoxEditor::tryGetRects(ut::cstrparam name, BoxRects& rects)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->getChanged())
        {
            box->clearChanged();
            rects = box->rects;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetOuter(cstrparam name, rect& outer)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->getChanged())
        {
            box->clearChanged();
            outer = box->rects.outer;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetBorder(cstrparam name, rect& border)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->getChanged())
        {
            box->clearChanged();
            border = box->rects.border;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetInner(cstrparam name, rect& inner)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->getChanged())
        {
            box->clearChanged();
            inner = box->rects.inner;
            return true;
        }
    }
    return false;
}

bool BoxEditor::draw()
{
    using namespace ImGui;

    if (!Box::isRoot(root_box))
        return false;

    PushID(m_name.c_str());
    drawMainWindow();
    Box::drawPropertiesWindow(*this);
    Box::drawOverlay(*this);
    PopID();

    if (m_name != GLOBAL_OPTIONS.active_editor_name)
    {
        is_overlay_visible = false;
        want_capture_mouse = false;
    }

    return true;
}

void BoxEditor::drawMenu()
{

}

void BoxEditor::drawMainWindow()
{
    using namespace ImGui;

    auto lbl = PRINTER("Box Editor [%s]###box_hierarchy_%s", m_name.c_str(), m_name.c_str());

    if (Begin(lbl))
    {
        drawMainWindowOptions();

        if (CollapsingHeader("File"))
            drawMainWindowFileOptions();

        if (CollapsingHeader(PRINTER("Binds [%d/%d]###binds", getFilledSlotCount(), getSlotCount())))
            drawMainWindowBindOptions();

        if (CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
            Box::drawBoxHierarchy(*this);
    }
    End();
}

void BoxEditor::drawMainWindowOptions()
{
    using namespace ImGui;

    if (ButtonActivated(ICON_LC_MOUSE_POINTER_SQUARE, m_name == GLOBAL_OPTIONS.active_editor_name))
    {
        GLOBAL_OPTIONS.active_editor_name = m_name;
        is_overlay_visible = true;
        want_capture_mouse = true;
    }

    if (IsItemHovered())
        SetTooltip("Select Editor [%s]", m_name.c_str());

    if (m_name == GLOBAL_OPTIONS.active_editor_name)
    {
        SameLine();
        if (Button(ICON_LC_X))
        {
            GLOBAL_OPTIONS.active_editor_name.clear();
        }

        if (IsItemHovered())
            SetTooltip("Unselect Editor");

        SameLine();
        if (ButtonSelected(is_overlay_visible ? ICON_LC_EYE "###vis" : ICON_LC_EYE_OFF "###vis", is_overlay_visible))
        {
            is_overlay_visible = !is_overlay_visible;
        }

        SameLine();
        if (ButtonSelected(ICON_LC_MOUSE "###input", want_capture_mouse))
        {
            want_capture_mouse = !want_capture_mouse;
        }

        if (!is_overlay_visible)
        {
            want_capture_mouse = false;
        }
    }

    SameLine();
    if (Button(ICON_LC_SETTINGS))
    {
        GLOBAL_OPTIONS.showDebugWindow();
    }

    if (IsItemHovered())
        SetTooltip("Global Editor Settings...");

}

void BoxEditor::drawMainWindowBoxEditOptions(BoxEditOptions &opts)
{
    using namespace ImGui;

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

    {
        auto& i = opts.style_index;
        if (ButtonActivated("Default" , i==0)) i=0; SameLine();
        if (ButtonActivated("Opaque"  , i==1)) i=1; SameLine();
        if (ButtonActivated("Fade"    , i==2)) i=2; SameLine();
        if (ButtonActivated("Hide"    , i==3)) i=3;
    }


    auto    style_default   = OverlayOptions::STYLE_PRESET_DEFAULT;
    auto&   style           = opts.style();

    if (ButtonDefault("bg_opacity", style.background.a != style_default.background.a))
    { style.background.a = style_default.background.a; }

    if (int o = int( float(style.background.a) / 255.0f * 100.0f); SliderInt("Opacity (BG)", &o, 0, 100, "%d%%"))
        style.background.a = b8( float(o) / 100.0f * 255 );

    if (ButtonDefault("background", style.background != style_default.background))
    { style.background = style_default.background; }

    ColorEdit3("Background", style.background);

    Dummy(GetItemRectSize());

    if (ButtonDefault("bd_opacity", style.border.a != style_default.border.a))
    { style.border.a = style_default.border.a; }

    if (int o = int( float(style.border.a) / 255.0f * 100.0f); SliderInt("Opacity (BD)", &o, 0, 100, "%d%%"))
        style.border.a = b8( float(o) / 100.0f * 255 );

    if (ButtonDefault("border", style.border != style_default.border))
    { style.border = style_default.border; }

    ColorEdit3("Border", style.border);

    Dummy(GetItemRectSize());

    Checkbox("Want Mouse Input", &want_capture_mouse);
}

void BoxEditor::drawMainWindowFileOptions()
{
    using namespace ImGui;

    if (m_current_file.empty())
    {
        TextDisabled("No current file");
    }
    else
    {
        TextUnformatted(m_current_file);
    }

    if (ButtonSelected(ICON_LC_SAVE, m_autosave))
    {
        m_autosave = !m_autosave;
    }

    if (IsItemHovered())
        SetTooltip("Autosave");

    SameLine();
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

void BoxEditor::drawMainWindowBindOptions()
{
    using namespace ImGui;

    ImGuiTableFlags table_flags =
        ImGuiTableFlags_BordersV            |
        ImGuiTableFlags_BordersOuterH       |
        ImGuiTableFlags_SizingFixedFit      |
        ImGuiTableFlags_ScrollX             |
        ImGuiTableFlags_RowBg               |
        ImGuiTableFlags_NoBordersInBody;

    if (BeginTable("slots", 2, table_flags, ImVec2{0, 150}))
    {
        TableSetupColumn("Name");
        TableSetupColumn("Box");
        TableHeadersRow();

        for (auto&& it : boxMap())
        {
            if (TableNextColumn())
                TextUnformatted(it.first);

            if (auto box = it.second)
            {
                if (TableNextColumn())
                {
                    PushID(it.first.c_str());
                    box->drawBreadcrumbs(*this);
                    PopID();
                }
            }
            else if (TableNextColumn())
            {
                TextUnformatted("---"_sv);
            }

        }

        EndTable();
    }

    if (auto num_empty_slots = getEmptySlotCount(); num_empty_slots == 0)
    {
        if (Button("to Console"))
        {
            auto s = BoxVisitor::toCPPString();
            fwrite(s.data(), sizeof(char), s.size(), stdout);
            fflush(stdout);
        }

        SameLine();

        if (Button("to Clipboard"))
        {
            auto s = BoxVisitor::toCPPString();
            SetClipboardText(s.c_str());
        }
    }
    else
    {
        PushItemDisabled();
        Button("to Console");
        SameLine();
        Button("to Clipboard");
        PopItemDisabled();
        SameLine();
        PushStyleColor(ImGuiCol_Text, colors::red);
        Text("%zu unassigned binds", num_empty_slots);
        PopStyleColor();
    }
}

void BoxEditor::loadFile(ut::cstrparam filename)
{
    box_ptr new_root = Box::createRoot(root_box->sizer);
    if (new_root->loadYaml(filename))
    {
        m_current_file          = filename;
        root_box                = new_root;

        setSelectedBoxSingle(root_box);

        resetAllSlots();
        setBoxSlotAll(root_box);
    }
}

void BoxEditor::saveFile(ut::cstrparam filename)
{
    if (root_box->saveYaml(filename))
    {
        m_current_file = filename;
    }
}

