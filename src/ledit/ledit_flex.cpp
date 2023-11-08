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
    weight      = 1;
    inner_pad   = 10;
}

#define CASE_SELECTABLE(x_) if (Selectable(#x_, type == (x_))) { changed=SELF; type = x_; }

Flex::ChangeType Flex::drawProperties()
{
    using namespace ImGui;

    ChangeType changed = NONE;

    if (ButtonDefault("inner pad", inner_pad != 10))
    { changed=SELF; inner_pad=10; }

    if (float p = inner_pad; DragFloat("inner_pad", &p, 0.1f, 0.0f))
    {
        if (p > 0) { changed=SELF; inner_pad=p; }
    }

    if (ButtonDefault("weight", weight != 1))
    { changed=SIBLINGS; weight = 1; }

    if (int w = weight; InputInt("weight", &w, 1))
    {
        if (w > 0) { changed=SIBLINGS; weight = w; }
    }

    if (ButtonDefault("type", type != BOX_HBOX))
    { changed=SELF; type = BOX_HBOX; }

    if (BeginCombo("Type", box_to_string(type)))
    {
        EXPAND_BOXTYPE(CASE_SELECTABLE)
        EndCombo();
    }

    return changed;
}

Flex::ChangeType Flex::drawRowControls(BoxEditOptions const& edit_opts)
{
    using namespace ImGui;

    ChangeType changed = NONE;

    if (edit_opts.show_row_weight)
    {
        Text("%d", weight);
        SameLine();

        if (SmallButton("+"))
        { changed=SIBLINGS; weight++; }
        SameLine();

        if (SmallButtonEnabled("-", weight > 1))
        { changed=SIBLINGS; weight--; }
        SameLine();
    }

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