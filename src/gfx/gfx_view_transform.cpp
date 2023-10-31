//
// Created by james on 10/27/23.
//

#include "gfx/gfx_view_transform.hpp"
using namespace gfx;

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

vec2 ViewTransform::realPoint(vec2 const& p) const { return translate.transformPoint(p); }
rect ViewTransform::realRect (rect const& r) const { return { realPoint(r.min), realPoint(r.max) }; }
vec2 ViewTransform::viewPoint(vec2 const& p) const { return translate_inverse.transformPoint(p); }
rect ViewTransform::viewRect (rect const& r) const { return { viewPoint(r.min), viewPoint(r.max) }; }

ViewTransform ViewTransform::create(rect const& viewport, float width, float height)
{
    auto viewport_size = viewport.size();

    auto scale = min( viewport_size.x / width, viewport_size.y / height );
    auto size  = vec2{ width*scale, height*scale };
    auto pos   = viewport.min + ( viewport_size - size ) / 2;

    ViewTransform vt;

    vt.translate = mat4::scaleTranslate(
        scale, scale, 1,
        pos.x, pos.y, 0
    );

    vt.translate_inverse = vt.translate.inverted();

    return vt;
}