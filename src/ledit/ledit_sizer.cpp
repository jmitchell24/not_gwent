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

void Sizer::draw()
{
    using namespace ImGui;

    //
    // Padding
    //

    if (Button("R##pad")) pad = monostate{}; SameLine();

    BeginGroup();
    switch (pad.index())
    {
        case PAD_NONE:
            if (Button("pad"))      pad.emplace<PAD_1>(); SameLine();
            if (Button("pad hv"))   pad.emplace<PAD_2>(); SameLine();
            if (Button("pad ltrb")) pad.emplace<PAD_4>();
            break;

        case PAD_1:
            DragFloat("pad", &get<PAD_1>(pad), 1,0,FLT_MAX);
            break;

        case PAD_2:
            DragFloat("horz", &get<PAD_2>(pad).x, 1,0,FLT_MAX);
            DragFloat("vert", &get<PAD_2>(pad).y, 1,0,FLT_MAX);
            break;

        case PAD_4:
            DragFloat("left"  , &get<PAD_4>(pad).x, 1,0,FLT_MAX);
            DragFloat("top"   , &get<PAD_4>(pad).y, 1,0,FLT_MAX);
            DragFloat("right" , &get<PAD_4>(pad).z, 1,0,FLT_MAX);
            DragFloat("bottom", &get<PAD_4>(pad).w, 1,0,FLT_MAX);
            break;
    }
    EndGroup();

    //
    // Scale
    //

    if (Button("R##scl")) scl = monostate{}; SameLine();

    BeginGroup();
    switch (scl.index())
    {
        case SCL_NONE:
            if (Button("aspect"))   scl.emplace<SCL_ASPECT>  (1); SameLine();
            if (Button("scale"))    scl.emplace<SCL_SCALE>   (1); SameLine();
            if (Button("scale xy")) scl.emplace<SCL_SCALE_XY>(1); SameLine();
            break;

        case SCL_ASPECT:
            DragFloat("aspect", &get<SCL_ASPECT>(scl), 0.01, 0.5, 2);
            break;

        case SCL_SCALE:
            SliderFloat("scale", &get<SCL_SCALE>(scl), 0, 1);
            break;

        case SCL_SCALE_XY:
            SliderFloat("scale x", &get<SCL_SCALE_XY>(scl).x, 0, 1);
            SliderFloat("scale y", &get<SCL_SCALE_XY>(scl).y, 0, 1);
            break;
    }
    EndGroup();

    //
    // Position
    //

    if (Button("R##pos")) pos = monostate{}; SameLine();

    BeginGroup();
    switch (pos.index())
    {
        case POS_NONE:
            if (Button("anchor")) pos.emplace<POS_ANCHOR>(ANCHOR_CC); SameLine();
            if (Button("pos xy")) pos.emplace<POS_XY>    ();          SameLine();
            break;

        case POS_ANCHOR:
#define ANCHOR_BUTTON(lbl_, anchor_) \
    if (get<POS_ANCHOR>(pos) == anchor_) \
    { \
        PushStyleColor(ImGuiCol_Button, ut::colors::goldenrod); \
        Button(lbl_); \
        PopStyleColor(); \
    }  \
    else if (Button(lbl_)) \
    { \
        get<POS_ANCHOR>(pos) = anchor_; \
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

    switch (scl.index())
    {
        case SCL_ASPECT:
            b = b.fitAspect(get<SCL_ASPECT>(scl));
            break;

        case SCL_SCALE:
            b = b.withSize(perc(get<SCL_SCALE>(scl)),
                           perc(get<SCL_SCALE>(scl)));
        break;


        case SCL_SCALE_XY:
            b = b.withSize(perc(get<SCL_SCALE_XY>(scl).x),
                           perc(get<SCL_SCALE_XY>(scl).y));
            break;
    }

    auto w = b.width();
    auto h = b.height();

    switch (pos.index())
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
    }

    switch (pad.index())
    {
        case PAD_1: return b.shrunk(get<1>(pad));
        case PAD_2: return b.shrunk(get<2>(pad).x,
                                    get<2>(pad).y);
        case PAD_4: return b.shrunk(get<3>(pad).x,
                                    get<3>(pad).y,
                                    get<3>(pad).z,
                                    get<3>(pad).w);
    }

    return b;
}