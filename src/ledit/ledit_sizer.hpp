//
// Created by james on 10/6/23.
//

#pragma once

#include "ledit/ledit_enums.hpp"

#include <ut/math.hpp>
#include <ut/string.hpp>

#include <optional>
#include <variant>

#define EXPAND_DIM(VAR) \
    VAR(dim, DIM_ASPECT  , DimAspect , "aspect" , float    ) \
    VAR(dim, DIM_PERCENT , DimPercent, "percent", ut::vec2 ) \
    VAR(dim, DIM_UNITS   , DimUnits  , "units"  , ut::vec2 )

#define EXPAND_PAD(VAR) \
    VAR(pad, PAD_ONE , Pad1, "pad1", float    ) \
    VAR(pad, PAD_TWO , Pad2, "pad2", ut::vec2 ) \
    VAR(pad, PAD_FOUR, Pad4, "pad4", ut::vec4 )

#define EXPAND_POS(VAR) \
    VAR(pos, POS_ANCHOR , PosAnchor , "anchor" , AnchorType) \
    VAR(pos, POS_PERCENT, PosPercent, "percent", ut::vec2  ) \
    VAR(pos, POS_UNITS  , PosUnits  , "units"  , ut::vec2  )

#define VAR_ENUM(a_, b_, c_, d_, e_) ,b_
#define VAR_TYPE(a_, b_, c_, d_, e_) ,e_
#define VAR_FUNC(a_, b_, c_, d_, e_) \
    inline e_ const& get##c_() const { return std::get<b_>(a_); } \
    inline e_& get##c_() { return std::get<b_>(a_); } \
    inline void set##c_(e_ const& x) { a_.emplace<b_>(x); }



namespace ledit
{
    struct Sizer
    {
        enum PadType { PAD_NONE EXPAND_PAD(VAR_ENUM) };
        using pad_type = std::variant<std::monostate EXPAND_PAD(VAR_TYPE)>;

        enum DimType { DIM_NONE EXPAND_DIM(VAR_ENUM) };
        using dim_type = std::variant<std::monostate EXPAND_DIM(VAR_TYPE)>;

        enum PosType { POS_NONE EXPAND_POS(VAR_ENUM) };
        using pos_type = std::variant<std::monostate EXPAND_POS(VAR_TYPE)>;

        pad_type pad;
        dim_type dim;
        pos_type pos;

        inline PadType padType() const { return (PadType)pad.index(); }
        inline DimType dimType() const { return (DimType)dim.index(); }
        inline PosType posType() const { return (PosType)pos.index(); }

        inline bool isDefault() const
        { return !(pad.index()==0 || dim.index()==0 || pos.index()==0); }

        EXPAND_PAD(VAR_FUNC)
        EXPAND_DIM(VAR_FUNC)
        EXPAND_POS(VAR_FUNC)

        void reset();
        void drawProperties();

        ut::rect getPad  (ut::rect parent) const;
        ut::vec2 getDim  (ut::rect parent) const;
        ut::rect getPos  (ut::rect parent, ut::vec2 sz) const;

        ut::rect getInner(ut::rect const& parent) const;
        ut::rect getOuter(ut::rect const& parent) const;

        struct Box { ut::rect parent, pad, content; };
        Box getBox(ut::rect const& parent) const;

        bool hasPad();
    };
}