//
// Created by james on 10/29/23.
//

#pragma once

//
// ut
//
#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

//
// std
//
#include <memory>
#include <vector>
#include <optional>
#include <map>
#include <ut/check.hpp>

namespace ledit
{
    class Box;
    struct BoxRects;
    using box_ptr   = std::shared_ptr<Box>;
    using box_cptr  = std::shared_ptr<Box const>;
    using boxmap_t  = std::map<std::string, box_ptr>;
    using boxlist_t = std::vector<box_ptr>;

    struct BoxRects
    {
        ut::rect outer;
        ut::rect border;
        ut::rect inner;
    };


}