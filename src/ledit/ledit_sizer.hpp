//
// Created by james on 10/6/23.
//

#pragma once

#include "ledit/ledit_enums.hpp"

#include <ut/math.hpp>
#include <ut/string.hpp>

#include <optional>
#include <variant>

#define EXPAND_SCL(VAR) \
    VAR(scl, SCL_ASPECT  , SclAspect , float    ) \
    VAR(scl, SCL_SCALE   , SclScale  , float    ) \
    VAR(scl, SCL_SCALE_XY, SclScaleXY, ut::vec2 )

#define EXPAND_PAD(VAR) \
    VAR(pad, PAD_ONE , Pad1, float    ) \
    VAR(pad, PAD_TWO , Pad2, ut::vec2 ) \
    VAR(pad, PAD_FOUR, Pad4, ut::vec4 )

#define EXPAND_POS(VAR) \
    VAR(pos, POS_ANCHOR, PosAnchor, AnchorType) \
    VAR(pos, POS_XY    , PosXY    , ut::vec2 )

#define VAR_ENUM(a_, b_, c_, d_) ,b_
#define VAR_TYPE(a_, b_, c_, d_) ,d_
#define VAR_FUNC(a_, b_, c_, d_) \
    inline d_ const& get##c_() const { return std::get<b_>(a_); } \
    inline d_& get##c_() { return std::get<b_>(a_); } \
    inline void set##c_(d_ const& x) { a_.emplace<b_>(x); }



namespace ledit
{
    struct Sizer
    {
        enum PadType { PAD_NONE EXPAND_PAD(VAR_ENUM) };
        using pad_type = std::variant<std::monostate EXPAND_PAD(VAR_TYPE)>;

        enum SclType { SCL_NONE EXPAND_SCL(VAR_ENUM)  };
        using scl_type = std::variant<std::monostate EXPAND_SCL(VAR_TYPE)>;

        enum PosType { POS_NONE EXPAND_POS(VAR_ENUM)  };
        using pos_type = std::variant<std::monostate EXPAND_POS(VAR_TYPE)>;

        pad_type pad;
        scl_type scl;
        pos_type pos;

        inline PadType padType() const { return (PadType)pad.index(); }
        inline SclType sclType() const { return (SclType)scl.index(); }
        inline PosType posType() const { return (PosType)pos.index(); }

        EXPAND_PAD(VAR_FUNC)
        EXPAND_SCL(VAR_FUNC)
        EXPAND_POS(VAR_FUNC)

        void drawProperties();

        ut::rect operator() (ut::rect const& parent) const;
    };
}