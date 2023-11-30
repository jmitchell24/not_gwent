//
// Created by james on 10/30/23.
//

#pragma once

#include "ledit/ledit_types.hpp"
#include "ledit/ledit_enums.hpp"

namespace ledit
{
    struct Flex
    {
        BoxType type      = BOX_HBOX;
        float   inner_pad = 10;

        void reset();

        bool drawProperties();
        bool drawRowControls(BoxEditOptions const& opts);
    };

}

