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

#define CASE_SELECTABLE(x_) if (Selectable(#x_, type == (x_))) { mark|=true; type = x_; }

bool Flex::drawProperties()
{
    using namespace ImGui;

    bool mark = false;

    if (ButtonDefault("weight", weight != 1))
    { mark|=true; weight = 1; }

    if (int w = weight; InputInt("weight", &w, 1))
    {
        if (w > 0) { mark|=true; weight = w; }
    }

    if (ButtonDefault("type", type != BOX_HBOX))
    { mark|=true; type = BOX_HBOX; }

    if (BeginCombo("Type", box_to_string(type)))
    {
        EXPAND_BOXTYPE(CASE_SELECTABLE)
        EndCombo();
    }

    return mark;
}

bool Flex::drawRowControls(BoxEditOptions const& edit_opts)
{
    using namespace ImGui;

    bool mark = false;

    if (edit_opts.show_row_weight)
    {
        Text("%d", weight);
        SameLine();

        if (SmallButton("+"))
            weight++;
        SameLine();

        if (SmallButtonEnabled("-", weight > 1))
            weight--;
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

    return mark;
}

#undef CASE_SELECTABLE