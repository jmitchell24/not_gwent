//
// Created by james on 12/6/23.
//

#include "ledit/ledit_options.hpp"
#include "ledit/ledit_yaml.hpp"
using namespace ledit;

//
// ImGui
//
#include "rlImGui/ed/TextEditor.h"
#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
#include <ut/algo.hpp>
using namespace ut;

//
// Helper Functions
//

GlobalOptions& GlobalOptions::instance()
{
    static GlobalOptions x;
    return x;
}

GlobalOptions::GlobalOptions()
{
    loadConfig();
    saveConfig();
}

void GlobalOptions::showDebugWindow()
{
    m_show_options_window = true;
}

void GlobalOptions::drawDebug()
{
    using namespace ImGui;

    if (m_show_options_window)
    {
        auto window_flags = ImGuiWindowFlags_NoDocking;

        if (Begin("BoxEditor Global Options", &m_show_options_window, window_flags))
        {
            bool changed = false;

            if (CollapsingHeader("Options", ImGuiTreeNodeFlags_DefaultOpen))
            {
                changed |= drawBoxEditOptions(box_edit_options);
            }

            if (CollapsingHeader("Overlay", ImGuiTreeNodeFlags_DefaultOpen))
            {
                changed |= drawOverlayOptions(overlay_options);
            }

            if (changed)
            {
                saveConfig();
            }
        }
        End();
    }
}

void GlobalOptions::drawDebugMenu()
{
    using namespace ImGui;

    if (BeginMenu("File"))
    {
        if (MenuItem("Settings..."))
        {
            m_show_options_window=true;
        }
        EndMenu();
    }
}

bool GlobalOptions::drawBoxEditOptions(BoxEditOptions &opts)
{
    using namespace ImGui;

    bool changed = false;

    Columns(3, "row-columns", false);
    changed |= Checkbox("add", &opts.show_row_add);
    NextColumn();
    changed |= Checkbox("delete", &opts.show_row_delete);
    NextColumn();
    changed |= Checkbox("move", &opts.show_row_move);
    NextColumn();
    changed |= Checkbox("rename", &opts.show_row_rename);
    NextColumn();
    changed |= Checkbox("weight", &opts.show_row_weight);
    NextColumn();
    changed |= Checkbox("type", &opts.show_row_type);
    Columns();

    return changed;
}

bool GlobalOptions::drawOverlayOptions(OverlayOptions& opts)
{
    using namespace ImGui;

    bool changed = false;

    {
        auto& i = opts.style_index;
        if (ButtonActivated("Default" , i==0)) { i=0; changed=true; } SameLine();
        if (ButtonActivated("Opaque"  , i==1)) { i=1; changed=true; } SameLine();
        if (ButtonActivated("Fade"    , i==2)) { i=2; changed=true; } SameLine();
        if (ButtonActivated("Hide"    , i==3)) { i=3; changed=true; }
    }


    auto    style_default   = OverlayOptions::STYLE_PRESET_DEFAULT;
    auto&   style           = opts.style();

    if (ButtonDefault("bg_opacity", style.background.a != style_default.background.a))
    { style.background.a = style_default.background.a; changed=true; }

    if (int o = int( float(style.background.a) / 255.0f * 100.0f); SliderInt("Opacity (BG)", &o, 0, 100, "%d%%"))
    { style.background.a = b8( float(o) / 100.0f * 255 ); changed=true; }

    if (ButtonDefault("background", style.background != style_default.background))
    { style.background = style_default.background; changed=true; }

    changed |= ColorEdit3("Background", style.background);

    Dummy(GetItemRectSize());

    if (ButtonDefault("bd_opacity", style.border.a != style_default.border.a))
    { style.border.a = style_default.border.a; changed=true; }

    if (int o = int( float(style.border.a) / 255.0f * 100.0f); SliderInt("Opacity (BD)", &o, 0, 100, "%d%%"))
    { style.border.a = b8( float(o) / 100.0f * 255 ); changed=true; }

    if (ButtonDefault("border", style.border != style_default.border))
    { style.border = style_default.border; changed=true; }

    changed |= ColorEdit3("Border", style.border);

    Dummy(GetItemRectSize());

    return changed;
}

auto FILENAME = "data/config/ledit.yaml"_sv;

void GlobalOptions::loadConfig()
{
    if (auto file = fopen(FILENAME, "r"))
    {
        try
        {
            auto text = gulp::file_to_string(file);
            auto node = YAML::Load(text);

            fromYaml(node, box_edit_options);
            fromYaml(node, overlay_options);
        }
        catch (YAML::Exception const &ex)
        {
            fprintf(stderr, "%s:%d:%d:\n    ", FILENAME.c_str(), ex.mark.line, ex.mark.column);
            fprintf(stderr, "YAML parse error: %s\n", ex.msg.c_str());
        }

        fclose(file);
    }
}

void GlobalOptions::saveConfig() const
{
    if (auto file = fopen(FILENAME, "w"))
    {
        YAML::Emitter em;
        em << YAML::BeginMap;
        emitYaml(em, box_edit_options);
        emitYaml(em, overlay_options);
        em << YAML::EndMap;

        fwrite(em.c_str(), sizeof(char), em.size(), file);
        fclose(file);
    }
}