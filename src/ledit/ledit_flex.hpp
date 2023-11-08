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
        enum ChangeType { NONE, SELF, SIBLINGS };

        BoxType     type        = BOX_HBOX;
        int         weight      = 1;
        float       inner_pad   = 10;

        void reset();

        ChangeType drawProperties();
        ChangeType drawRowControls(BoxEditOptions const& opts);
    };

}