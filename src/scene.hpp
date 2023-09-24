//
// Created by james on 9/23/23.
//

#pragma once

#include <ut/math.hpp>

class Scene
{
public:
    virtual ~Scene() = default;
    virtual char const* name() const=0;

    virtual void layout(ut::rect const& b)=0;
    virtual void update(float dt)=0;
    virtual void draw()=0;
    virtual void drawDebug()=0;
};