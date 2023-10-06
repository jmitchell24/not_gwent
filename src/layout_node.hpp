//
// Created by james on 10/6/23.
//

#pragma once

//
// ut
//
#include <ut/math.hpp>

//
// std
//
#include <vector>
#include <array>
#include <cstddef>
#include <memory>

namespace layout
{
    class Node;
    class VBox;
    class HBox;
    class StackBox;
    class FillRect;
    class AnchorRect;

    using node_ptr          = std::shared_ptr<Node>;
    using vbox_ptr          = std::shared_ptr<VBox>;
    using hbox_ptr          = std::shared_ptr<HBox>;
    using stackbox_ptr      = std::shared_ptr<StackBox>;
    using fillrect_ptr      = std::shared_ptr<FillRect>;
    using anchorrect_ptr    = std::shared_ptr<AnchorRect>;

    using nodelist_t        = std::vector<node_ptr>;

    class Node
    {
    public:
        float inner_pad;
        float outer_pad;

        ut::rect bounds;


        virtual void layout(ut::rect const& b)=0;

        virtual void draw();
    private:
    };


}