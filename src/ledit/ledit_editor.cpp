//
// Created by james on 10/19/23.
//

#include "ledit/ledit_editor.hpp"
using namespace ledit;

//
// layout
//
using namespace layout;

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
    m_root_box_revert = Box::createRoot(bounds);

    clearSelectedBoxSingle();
}

void BoxEditor::setFile(ut::cstrparam file)
{
    loadFile(file);
}

bool BoxEditor::tryGetBox(cstrparam name, layout::LayoutBox& lb)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->wantBind())
        {
            box->clearWantBind();
            lb = box->lb;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetOuter(cstrparam name, rect& outer)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->wantBind())
        {
            box->clearWantBind();
            outer = box->lb.outer;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetBorder(cstrparam name, rect& border)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->wantBind())
        {
            box->clearWantBind();
            border = box->lb.border;
            return true;
        }
    }
    return false;
}

bool BoxEditor::tryGetInner(cstrparam name, rect& inner)
{
    if (auto box = getBoxSlot(name))
    {
        if (box->wantBind())
        {
            box->clearWantBind();
            inner = box->lb.inner;
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

    setGlobalInstance();

    PushID(m_name.c_str());
    drawMainWindow();
    Box::drawPropertiesWindow();
    Box::drawOverlay();
    PopID();

    if (!isActive())
    {
        is_overlay_visible = false;
        want_capture_mouse = false;
    }

    clearGlobalInstance();

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
        if (!LEDIT_OPTIONS.hasActive() && m_want_active)
            setActive();

        drawMainWindowOptions();

        auto file_lbl = want_persist ? "File (modified)###file" : "File###file";
        auto file_selected = want_persist;
        if (CollapsingHeaderSelected(file_lbl, file_selected, 0, WC_YELLOW))
            drawMainWindowFileOptions();

        auto binds_lbl = PRINTER("Binds [%d/%d]###binds", getFilledSlotCount(), getSlotCount());
        auto binds_selected = getEmptySlotCount() > 0;
        if (CollapsingHeaderSelected(binds_lbl, binds_selected, 0, WC_RED))
            drawMainWindowBindOptions();

        if (CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
            Box::drawBoxHierarchy();
    }
    else
    {
        if (isActive())
            clearActive();
    }
    End();
}

void BoxEditor::drawMainWindowOptions()
{
    using namespace ImGui;

    PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{3.0f,0.0f});
    BeginGroup();

    if (ButtonActivated(ICON_LC_MOUSE_POINTER_SQUARE, m_name == LEDIT_OPTIONS.active_editor_name))
    {
        setActive();
        m_want_active = true;
    }

    if (IsItemHovered())
        SetTooltip("Select Editor [%s]", m_name.c_str());

    if (m_name == LEDIT_OPTIONS.active_editor_name)
    {
        SameLine();
        if (Button(ICON_LC_X, WC_RED))
        {
            LEDIT_OPTIONS.clearActive();
            m_want_active = false;
        }

        if (IsItemHovered()) SetTooltip("Unselect Editor");

        SameLine();
        if (ButtonSelected(is_overlay_visible ? ICON_LC_EYE "###vis" : ICON_LC_EYE_OFF "###vis", is_overlay_visible, WC_GREEN))
        {
            is_overlay_visible = !is_overlay_visible;
        }

        if (IsItemHovered()) SetTooltip("Is Overlay Visible?");

        SameLine();
        if (ButtonSelected(ICON_LC_MOUSE "###input", want_capture_mouse, WC_GREEN))
        {
            want_capture_mouse = !want_capture_mouse;
        }

        if (IsItemHovered()) SetTooltip("Is Overlay Interaction Enabled?");

        if (!is_overlay_visible)
        {
            want_capture_mouse = false;
        }
    }

    EndGroup();
    PopStyleVar(2);

    SameLine();
    if (Button(ICON_LC_SETTINGS))
    {
        LEDIT_OPTIONS.showDebugWindow();
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

    SameLine();
    if (want_persist)
    {
        if (ButtonConfirm("Overwrite"))
        {
            saveFile(m_current_file);
        }

        SameLine();
        if (ButtonConfirm("Revert"))
        {
            root_box = m_root_box_revert->deepCopy(nullptr);
            setSelectedBoxSingle(root_box);
            resetAllSlots();
            setBoxSlotAll(root_box);

            want_persist = false;
        }
    }
    else
    {
        ButtonEnabled("Overwrite", false);
        SameLine();
        ButtonEnabled("Revert", false);
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
                    box->drawBreadcrumbs();
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

    if (ButtonConfirm("Reset"))
    {
        m_box_map.clear();
    }

    SameLine();

    if (auto num_empty_slots = getEmptySlotCount(); num_empty_slots == 0)
    {
        if (Button("Preview"))
        {
            OpenPopup("cpp_code_preview");
        }

        SameLine();

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

    if (BeginPopup("cpp_code_preview"))
    {
        EndPopup();
    }
}

void BoxEditor::loadFile(ut::cstrparam filename)
{
    box_ptr new_root = Box::createRoot(root_box->sizer);
    if (new_root->loadYaml(filename))
    {
        m_current_file          = filename;
        root_box                = new_root;
        m_root_box_revert       = root_box->deepCopy(nullptr);
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
        m_root_box_revert = root_box->deepCopy(nullptr);
        want_persist = false;
    }
}

