//
// Created by james on 10/6/23.
//

#include "ledit/ledit_sizer.hpp"
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
// Helpers
//



//
// Sizer -> Implementation
//

void Sizer::reset()
{
    pad = monostate{};
    dim = monostate{};
    pos = monostate{};
}

bool Sizer::drawProperties()
{
    using namespace ImGui;

    bool changed=false;

    //
    // Margin
    //

    if (ButtonDefault("Margin", mrg.index()>0))
    { changed=true; mrg = monostate{}; }

    BeginGroup();
    switch (mrgType())
    {
        case MRG_NONE:
            if (BeginCombo("Margin", "..."))
            {
                if (Selectable("Margin"     )) { changed=true; setMrg1({}); }
                if (Selectable("Margin hv"  )) { changed=true; setMrg2({}); }
                if (Selectable("Margin ltrb")) { changed=true; setMrg4({}); }
                EndCombo();
            }
            break;

        case MRG_ONE:
            changed|=DragFloat("Margin", &getMrg1(), 1,0,FLT_MAX);
            break;

        case MRG_TWO:
            changed|=DragFloat("Margin horz", &getMrg2().x, 1,0,FLT_MAX);
            changed|=DragFloat("Margin vert", &getMrg2().y, 1,0,FLT_MAX);
            break;

        case MRG_FOUR:
            changed|=DragFloat("Margin left"  , &getMrg4().x, 1,0,FLT_MAX);
            changed|=DragFloat("Margin top"   , &getMrg4().y, 1,0,FLT_MAX);
            changed|=DragFloat("Margin right" , &getMrg4().z, 1,0,FLT_MAX);
            changed|=DragFloat("Margin bottom", &getMrg4().w, 1,0,FLT_MAX);
            break;
    }
    EndGroup();

    //
    // Padding
    //

    if (ButtonDefault("Padding", pad.index()>0))
    { changed=true; pad = monostate{}; }

    BeginGroup();
    switch (padType())
    {
        case PAD_NONE:
            if (BeginCombo("Padding", "..."))
            {
                if (Selectable("Padding"     )) { changed=true; setPad1({}); }
                if (Selectable("Padding hv"  )) { changed=true; setPad2({}); }
                if (Selectable("Padding ltrb")) { changed=true; setPad4({}); }
                EndCombo();
            }
            break;

        case PAD_ONE:
            changed|=DragFloat("Padding", &getPad1(), 1,0,FLT_MAX);
            break;

        case PAD_TWO:
            changed|=DragFloat("Padding horz", &getPad2().x, 1,0,FLT_MAX);
            changed|=DragFloat("Padding vert", &getPad2().y, 1,0,FLT_MAX);
            break;

        case PAD_FOUR:
            changed|=DragFloat("Padding left"  , &getPad4().x, 1,0,FLT_MAX);
            changed|=DragFloat("Padding top"   , &getPad4().y, 1,0,FLT_MAX);
            changed|=DragFloat("Padding right" , &getPad4().z, 1,0,FLT_MAX);
            changed|=DragFloat("Padding bottom", &getPad4().w, 1,0,FLT_MAX);
            break;
    }
    EndGroup();

    //
    // Scale
    //

    if (ButtonDefault("Dimensions", dim.index()>0))
    { changed=true; dim = monostate{}; }

    BeginGroup();
    switch (dimType())
    {
        case DIM_NONE:
            if (BeginCombo("Dimensions", "..."))
            {
                if (Selectable("Aspect" )) { changed=true; setDimAspect (1);              }
                if (Selectable("Percent")) { changed=true; setDimPercent({1,1});          }
                if (Selectable("Units"  )) { changed=true; setDimUnits  ({100,100});      }
                EndCombo();
            }
            break;

        case DIM_ASPECT:
            changed|=DragFloat("Aspect", &getDimAspect(), 0.01, 0.5, 2);
            break;

        case DIM_PERCENT:
            changed|=SliderFloat("Percent W", &getDimPercent().x, 0, 1);
            changed|=SliderFloat("Percent H", &getDimPercent().y, 0, 1);
            break;

        case DIM_UNITS:
            changed|=DragFloat("Units W", &getDimUnits().x, 1, 0, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            changed|=DragFloat("Units H", &getDimUnits().y, 1, 0, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            break;
    }
    EndGroup();

    //
    // Position
    //

    if (ButtonDefault("Position", pos.index()>0))
    { changed=true; pos = monostate{}; }

    BeginGroup();
    switch (posType())
    {
        case POS_NONE:
            if (BeginCombo("Position", "..."))
            {
                if (Selectable("Anchor" )) { changed=true; setPosAnchor(ANCHOR_CC);   }
                if (Selectable("Percent")) { changed=true; setPosPercent({});         }
                if (Selectable("Units"  )) { changed=true; setPosUnits({});           }
                EndCombo();
            }
            break;

        case POS_ANCHOR:
#define ANCHOR_BUTTON(lbl_, anchor_) \
    if (getPosAnchor() == (anchor_)) \
    { \
        PushStyleColor(ImGuiCol_Button, ut::colors::goldenrod); \
        Button((lbl_)); \
        PopStyleColor(); \
    }  \
    else if (Button((lbl_))) \
    { changed=true; getPosAnchor() = (anchor_); }

        ANCHOR_BUTTON("TL", ANCHOR_TL) SameLine();
        ANCHOR_BUTTON("TC", ANCHOR_TC) SameLine();
        ANCHOR_BUTTON("TR", ANCHOR_TR)

        ANCHOR_BUTTON("LC", ANCHOR_LC) SameLine();
        ANCHOR_BUTTON("CC", ANCHOR_CC) SameLine();
        ANCHOR_BUTTON("RC", ANCHOR_RC)

        ANCHOR_BUTTON("BL", ANCHOR_BL) SameLine();
        ANCHOR_BUTTON("BC", ANCHOR_BC) SameLine();
        ANCHOR_BUTTON("BR", ANCHOR_BR)
#undef ANCHOR_BUTTON
            break;

        case POS_PERCENT:
            changed|=SliderFloat("Percent X", &getPosPercent().x, 0, 1);
            changed|=SliderFloat("Percent Y", &getPosPercent().y, 0, 1);
            break;

        case POS_UNITS:
            changed|=DragFloat("Units X", &getPosUnits().x, 1, -FLT_MAX, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            changed|=DragFloat("Units Y", &getPosUnits().y, 1, -FLT_MAX, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            break;
    }
    EndGroup();

    return changed;
}

rect Sizer::getMrg(rect b) const
{
    switch (mrgType())
    {
        case MRG_NONE: return b;
        case MRG_ONE : return b.deflated(getMrg1());
        case MRG_TWO : return b.deflated(getMrg2().x, getMrg2().y);
        case MRG_FOUR: return b.deflated(getMrg4().x, getMrg4().y, getMrg4().z, getMrg4().w);
        default:nopath_case(MrgType);
    }
    return b;
}

rect Sizer::getPad(rect b) const
{
    switch (padType())
    {
        case PAD_NONE: return b;
        case PAD_ONE : return b.deflated(getPad1());
        case PAD_TWO : return b.deflated(getPad2().x, getPad2().y);
        case PAD_FOUR: return b.deflated(getPad4().x, getPad4().y, getPad4().z, getPad4().w);
        default:nopath_case(PadType);
    }
    return b;
}

vec2 Sizer::getDim(rect b) const
{
    switch (dimType())
    {
        case DIM_NONE    : return b.size();
        case DIM_ASPECT  : return b.fitAspectSize(getDimAspect());
        case DIM_PERCENT : return b.size() * getDimPercent();
        case DIM_UNITS   : return getDimUnits();
        default:nopath_case(DimType);
    }
    return b.size();
}

rect Sizer::getPos(rect b, vec2 sz) const
{
    switch (posType())
    {
        case POS_ANCHOR:
            switch (getPosAnchor())
            {
                case ANCHOR_TL: return b.anchorTLtoTL(sz);
                case ANCHOR_TR: return b.anchorTRtoTR(sz);
                case ANCHOR_BL: return b.anchorBLtoBL(sz);
                case ANCHOR_BR: return b.anchorBRtoBR(sz);
                case ANCHOR_LC: return b.anchorLCtoLC(sz);
                case ANCHOR_RC: return b.anchorRCtoRC(sz);
                case ANCHOR_TC: return b.anchorTCtoTC(sz);
                case ANCHOR_BC: return b.anchorBCtoBC(sz);
                case ANCHOR_CC: return b.anchorCCtoCC(sz);
                default:nopath_case(AnchorType);
            }
            break;

        case POS_PERCENT:
            return b.withSize(sz).withOffset( (b.size() - sz) * getPosPercent() );

        case POS_UNITS:
            return b.withSize(sz).withOffset(getPosUnits());
        default:break;
    }

    return b.withSize(sz);
}

void Sizer::getBoxRects(rect const& parent, BoxRects& child) const
{
    auto sz = getDim(parent);
    child.outer  = getPos(parent, sz);
    child.border = getMrg(child.outer);
    child.inner  = getPad(child.border);
}