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
#include <ut/check.hpp>
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
    // Padding
    //

    if (ButtonDefault("pad", pad.index()>0))
    { changed=true; pad = monostate{}; }

    BeginGroup();
    switch (padType())
    {
        case PAD_NONE:
            if (BeginCombo("pad", "..."))
            {
                if (Selectable("pad"     )) { changed=true; setPad1({}); }
                if (Selectable("pad hv"  )) { changed=true; setPad2({}); }
                if (Selectable("pad ltrb")) { changed=true; setPad4({}); }
                EndCombo();
            }
            break;

        case PAD_ONE:
            changed|=DragFloat("pad", &getPad1(), 1,0,FLT_MAX);
            break;

        case PAD_TWO:
            changed|=DragFloat("horz", &getPad2().x, 1,0,FLT_MAX);
            changed|=DragFloat("vert", &getPad2().y, 1,0,FLT_MAX);
            break;

        case PAD_FOUR:
            changed|=DragFloat("left"  , &getPad4().x, 1,0,FLT_MAX);
            changed|=DragFloat("top"   , &getPad4().y, 1,0,FLT_MAX);
            changed|=DragFloat("right" , &getPad4().z, 1,0,FLT_MAX);
            changed|=DragFloat("bottom", &getPad4().w, 1,0,FLT_MAX);
            break;
    }
    EndGroup();

    //
    // Scale
    //

    if (ButtonDefault("dim", dim.index()>0))
    { changed=true; dim = monostate{}; }

    BeginGroup();
    switch (dimType())
    {
        case DIM_NONE:
            if (BeginCombo("dim", "..."))
            {
                if (Selectable("aspect" )) { changed=true; setDimAspect (1);              }
                if (Selectable("percent")) { changed=true; setDimPercent({1,1});          }
                if (Selectable("units"  )) { changed=true; setDimUnits  ({100,100});      }
                EndCombo();
            }
            break;

        case DIM_ASPECT:
            changed|=DragFloat("aspect", &getDimAspect(), 0.01, 0.5, 2);
            break;

        case DIM_PERCENT:
            changed|=SliderFloat("percent w", &getDimPercent().x, 0, 1);
            changed|=SliderFloat("percent h", &getDimPercent().y, 0, 1);
            break;

        case DIM_UNITS:
            changed|=DragFloat("units w", &getDimUnits().x, 1, 0, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            changed|=DragFloat("units h", &getDimUnits().y, 1, 0, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            break;
    }
    EndGroup();

    //
    // Position
    //

    if (ButtonDefault("pos", pos.index()>0))
    { changed=true; pos = monostate{}; }

    BeginGroup();
    switch (posType())
    {
        case POS_NONE:
            if (BeginCombo("pos", "..."))
            {
                if (Selectable("anchor" )) { changed=true; setPosAnchor(ANCHOR_CC);   }
                if (Selectable("percent")) { changed=true; setPosPercent({});         }
                if (Selectable("units"  )) { changed=true; setPosUnits({});           }
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
            changed|=SliderFloat("percent x", &getPosPercent().x, 0, 1);
            changed|=SliderFloat("percent y", &getPosPercent().y, 0, 1);
            break;

        case POS_UNITS:
            changed|=DragFloat("units x", &getPosUnits().x, 1, -FLT_MAX, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            changed|=DragFloat("units y", &getPosUnits().y, 1, -FLT_MAX, FLT_MAX, "%.0f", ImGuiSliderFlags_AlwaysClamp);
            break;
    }
    EndGroup();
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

//rect Sizer::getOuter(rect const& parent) const
//{
//    auto sz = getDim(parent);
//    return getPos(parent, sz);
//}
//
//rect Sizer::getInner(rect const& parent) const
//{
//    return getPad(getOuter(parent));
//}

void Sizer::getInnerOuter(rect const& parent, rect& inner, rect& outer) const
{
    auto sz = getDim(parent);
    outer   = getPos(parent, sz);
    inner   = getPad(outer);
}