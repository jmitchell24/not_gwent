//
// Created by james on 10/8/23.
//

#pragma once

#include <ut/string.hpp>

#define EXPAND_ANCHORTYPE(CASE) \
    CASE(ANCHOR_TL) \
    CASE(ANCHOR_TR) \
    CASE(ANCHOR_BL) \
    CASE(ANCHOR_BR) \
    CASE(ANCHOR_LC) \
    CASE(ANCHOR_RC) \
    CASE(ANCHOR_TC) \
    CASE(ANCHOR_BC) \
    CASE(ANCHOR_CC)

#define EXPAND_BOXTYPE(CASE) \
    CASE(BOX_VBOX) \
    CASE(BOX_HBOX) \
    CASE(BOX_SBOX)

#define CASE_ENUM(x_) x_,
#define CASE_ENUM_TO_STRING(x_) case x_: return #x_##_sv;
#define CASE_STRING_TO_ENUM(x_) if (s == #x_##_sv) { x = x_; return true; }
#define CASE_SELECTABLE(x_) if (Selectable(#x_, type == x_)) { type = x_; }

namespace ledit
{
    enum AnchorType     { EXPAND_ANCHORTYPE(CASE_ENUM) };
    enum BoxType        { EXPAND_BOXTYPE(CASE_ENUM) };

    inline static ut::cstrview anchor_to_string(AnchorType x)
    {
        using namespace ut;
        switch (x) { EXPAND_ANCHORTYPE(CASE_ENUM_TO_STRING) }
        return ""_sv;
    }

    inline static bool string_to_anchor(ut::cstrparam s, AnchorType& x)
    {
        using namespace ut;
        EXPAND_ANCHORTYPE(CASE_STRING_TO_ENUM)
        return false;
    }

    inline static ut::cstrview box_to_string(BoxType x)
    {
        using namespace ut;
        switch (x) { EXPAND_BOXTYPE(CASE_ENUM_TO_STRING) }
        return ""_sv;
    }

    inline static bool string_to_box(ut::cstrparam s, BoxType& x)
    {
        using namespace ut;
        EXPAND_BOXTYPE(CASE_STRING_TO_ENUM)
        return false;
    }
}