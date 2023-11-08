//
// Created by james on 10/27/23.
//


#include "context.hpp"
#include "conv.hpp"

//
// gfx
//
using namespace gfx;

//
// raylib
//
#include <raylib.h>

Context::UpdateData Context::createUpdateData(ViewTransform const& vt)
{
    auto mp = tout(GetMousePosition());

    return UpdateData {
        .frame_time         = GetFrameTime(),
        .real_mouse_pos     = mp,
        .view_mouse_pos     = vt.viewPoint(mp),
        .view_transform     = vt
    };
}

Context& Context::instance()
{
    static Context x;
    return x;
}