//
// Created by james on 10/10/23.
//

#pragma once

#include <ut/math.hpp>

#define EXPAND_CARD_BIND(BIND) \
    BIND(0, a.b.c)

namespace ledit
{
    struct Layout
    {
        using rectlist_t = std::vector<ut::rect>;

        ut::rect    root;
        rectlist_t  bounds;


    };

    class Bind
    {
    public:

    private:

    };
}
