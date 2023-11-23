//
// Created by james on 9/23/23.
//

#pragma once

#include "context.hpp"

//
// ut
//
#include <ut/math.hpp>
#include <ut/string.hpp>

class Scene
{
public:
    virtual ~Scene() = default;
    [[nodiscard]] virtual ut::cstrview name() const=0;

    virtual void load       (){}
    virtual void unload     (){}
    virtual void layout     (){}
    virtual void update     (update_param u){}
    virtual void draw       (){}
    virtual void drawDebug  (){}
    virtual void drawDebugMenu() {}
};