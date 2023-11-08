//
// Created by james on 10/27/23.
//
#pragma once

#include "gfx/gfx_view_transform.hpp"

class Context
{
public:
    struct UpdateData
    {
        float       frame_time;
        ut::vec2    real_mouse_pos;
        ut::vec2    view_mouse_pos;

        gfx::ViewTransform view_transform;
    };

    static constexpr size_t     VIEW_WIDTH  { 1280 };
    static constexpr size_t     VIEW_HEIGHT { 720  };
    static constexpr ut::rect   VIEW_RECT   { 0, 0, VIEW_WIDTH, VIEW_HEIGHT };

    [[nodiscard]] static UpdateData createUpdateData(gfx::ViewTransform const& vt);

    static Context& instance();

private:
    Context()=default;
};

using update_param = Context::UpdateData const&;

static Context& CONTEXT = Context::instance();


