//
// Created by james on 10/16/23.
//

#pragma once

#include "ledit/ledit_box.hpp"

namespace ledit
{
    class Layout
    {
    public:
        float width  =1280;
        float height = 720;

        box_ptr root_box;
        box_ptr selected_box;

        bool loadYaml(ut::cstrparam filename);
        bool saveYaml(ut::cstrparam filename);

        void drawProperties();

    private:
        Layout();
    };
}