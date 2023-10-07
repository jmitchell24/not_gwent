//
// Created by james on 10/6/23.
//

#include <ut/math.hpp>

#include <optional>
#include <variant>

namespace ledit
{


    struct Sizer
    {
        enum Anchor
        {
            ANCHOR_TL,
            ANCHOR_TR,
            ANCHOR_BL,
            ANCHOR_BR,

            ANCHOR_LC,
            ANCHOR_RC,
            ANCHOR_TC,
            ANCHOR_BC,

            ANCHOR_CC
        };

        enum PadType { PAD_NONE, PAD_1, PAD_2, PAD_4 };
        using pad_type = std::variant<std::monostate, float, ut::vec2, ut::vec4>;

        enum SclType { SCL_NONE, SCL_ASPECT, SCL_SCALE, SCL_SCALE_XY };
        using scl_type = std::variant<std::monostate, float, float, ut::vec2>;

        enum PosType { POS_NONE, POS_ANCHOR, POS_XY };
        using pos_type = std::variant<std::monostate, Anchor, ut::vec2>;

        pad_type    pad;
        scl_type    scl;
        pos_type    pos;

        void draw();

        ut::rect operator() (ut::rect const& parent) const;
    };
}