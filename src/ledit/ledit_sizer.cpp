//
// Created by james on 10/6/23.
//

#include "ledit/ledit_sizer.hpp"
using namespace ledit;

#include "assert_msg.hpp"

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
// Sizer -> Implementation
//

void Sizer::reset()
{
    pad = monostate{};
    scl = monostate{};
    pos = monostate{};
}

void Sizer::drawProperties()
{
    using namespace ImGui;

    //
    // Padding
    //

    if (ButtonEnabled("R##pad", pad.index()>0)) pad = monostate{}; SameLine();

    BeginGroup();
    switch (padType())
    {
        case PAD_NONE:
            if (Button("pad"))      setPad1({}); SameLine();
            if (Button("pad hv"))   setPad2({}); SameLine();
            if (Button("pad ltrb")) setPad4({});
            break;

        case PAD_ONE:
            DragFloat("pad", &getPad1(), 1,0,FLT_MAX);
            break;

        case PAD_TWO:
            DragFloat("horz", &getPad2().x, 1,0,FLT_MAX);
            DragFloat("vert", &getPad2().y, 1,0,FLT_MAX);
            break;

        case PAD_FOUR:
            DragFloat("left"  , &getPad4().x, 1,0,FLT_MAX);
            DragFloat("top"   , &getPad4().y, 1,0,FLT_MAX);
            DragFloat("right" , &getPad4().z, 1,0,FLT_MAX);
            DragFloat("bottom", &getPad4().w, 1,0,FLT_MAX);
            break;
    }
    EndGroup();

    //
    // Scale
    //

    if (ButtonEnabled("R##scl", scl.index()>0)) scl = monostate{}; SameLine();

    BeginGroup();
    switch (sclType())
    {
        case SCL_NONE:
            if (Button("aspect"))   setSclAspect (1);     SameLine();
            if (Button("scale"))    setSclScale  (1);     SameLine();
            if (Button("scale xy")) setSclScaleXY({1,1});
            break;

        case SCL_ASPECT:
            DragFloat("aspect", &getSclAspect(), 0.01, 0.5, 2);
            break;

        case SCL_SCALE:
            SliderFloat("scale", &getSclScale(), 0, 1);
            break;

        case SCL_SCALE_XY:
            SliderFloat("scale x", &getSclScaleXY().x, 0, 1);
            SliderFloat("scale y", &getSclScaleXY().y, 0, 1);
            break;
    }
    EndGroup();

    //
    // Position
    //

    if (ButtonEnabled("R##pos", pos.index()>0)) pos = monostate{}; SameLine();

    BeginGroup();
    switch (posType())
    {
        case POS_NONE:
            if (Button("anchor")) setPosAnchor(ANCHOR_CC); SameLine();
            if (Button("pos xy")) setPosXY({});
            break;

        case POS_ANCHOR:
#define ANCHOR_BUTTON(lbl_, anchor_) \
    if (getPosAnchor() == anchor_) \
    { \
        PushStyleColor(ImGuiCol_Button, ut::colors::goldenrod); \
        Button(lbl_); \
        PopStyleColor(); \
    }  \
    else if (Button(lbl_)) \
    { \
        getPosAnchor() = anchor_; \
    }

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

        case POS_XY:
            SliderFloat("pos x", &get<POS_XY>(pos).x, 0, 1);
            SliderFloat("pos y", &get<POS_XY>(pos).y, 0, 1);
            break;
    }
    EndGroup();
}

rect Sizer::operator() (rect const& parent) const
{
    auto b = parent;

    switch (sclType())
    {
        case SCL_ASPECT:
            b = b.fitAspect(getSclAspect());
            break;

        case SCL_SCALE:
            b = b.withSize(perc(getSclScale()),
                           perc(getSclScale()));
        break;


        case SCL_SCALE_XY:
            b = b.withSize(perc(getSclScaleXY().x),
                           perc(getSclScaleXY().y));
            break;
        default:break;
    }

    auto w = b.width();
    auto h = b.height();

    switch (posType())
    {
        case POS_ANCHOR:
            switch (get<POS_ANCHOR>(pos))
            {
                case ANCHOR_TL: b = parent.anchorTLtoTL(w,h); break;
                case ANCHOR_TR: b = parent.anchorTRtoTR(w,h); break;
                case ANCHOR_BL: b = parent.anchorBLtoBL(w,h); break;
                case ANCHOR_BR: b = parent.anchorBRtoBR(w,h); break;
                case ANCHOR_LC: b = parent.anchorLCtoLC(w,h); break;
                case ANCHOR_RC: b = parent.anchorRCtoRC(w,h); break;
                case ANCHOR_TC: b = parent.anchorTCtoTC(w,h); break;
                case ANCHOR_BC: b = parent.anchorBCtoBC(w,h); break;
                case ANCHOR_CC: b = parent.anchorCCtoCC(w,h); break;
            }
            break;
        default:break;
    }

    switch (padType())
    {
        case PAD_ONE : return b.shrunk(getPad1());
        case PAD_TWO : return b.shrunk(getPad2().x,
                                       getPad2().y);
        case PAD_FOUR: return b.shrunk(getPad4().x,
                                       getPad4().y,
                                       getPad4().z,
                                       getPad4().w);
        default:break;
    }

    return b;
}