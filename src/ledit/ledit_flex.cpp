//
// Created by james on 10/30/23.
//

#include "ledit/ledit_flex.hpp"
using namespace ledit;

//
// imgui
//
#include "rlImGui/imgui/imgui_mods.hpp"

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

//
// Flex -> Implementation
//

void Flex::reset()
{
    type        = BOX_HBOX;
    inner_pad   = 10;
}

#define CASE_SELECTABLE(x_) if (Selectable(#x_, type == (x_))) { changed=true; type = x_; }

bool Flex::drawProperties()
{
    using namespace ImGui;

    bool changed = false;

    if (ButtonDefault("inner pad", inner_pad != 10))
    { changed=true; inner_pad=10; }

    changed|=DragFloat("inner_pad", &inner_pad, 1,0,FLT_MAX);

    if (ButtonDefault("type", type != BOX_HBOX))
    { changed=true; type = BOX_HBOX; }

    if (BeginCombo("Type", box_to_string(type)))
    {
        EXPAND_BOXTYPE(CASE_SELECTABLE)
        EndCombo();
    }

    return changed;
}

bool Flex::drawRowControls(BoxEditOptions const& edit_opts)
{
    using namespace ImGui;

    bool changed = false;

    if (edit_opts.show_row_type)
    {
        if (SmallButton("t"))
            OpenPopup("show-row-type");

        if (BeginPopup("show-row-type"))
        {
            EXPAND_BOXTYPE(CASE_SELECTABLE)
            EndPopup();
        }
        SameLine();
    }

    return changed;
}

#undef CASE_SELECTABLE