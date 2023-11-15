//
// Created by james on 10/29/23.
//

#include "ledit/ledit_box.hpp"
#include "ledit/ledit_box_visitor.hpp"
using namespace ledit;


//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

//
// BoxVisitor -> Implementation
//

boxmap_t const& BoxVisitor::boxMap() const
{
    return m_box_map;
}

rect BoxVisitor::getRealRect(rect const& r) const
{
    if (view_transform)
    {
        return view_transform->realRect(r);
    }
    return r;
}

vec2 BoxVisitor::getViewPoint(vec2 const& p) const
{
    if (view_transform)
    {
        return view_transform->viewPoint(p);
    }
    return p;
}

box_ptr BoxVisitor::getBoxSlot(cstrparam s)
{
    auto k = s.str();

    if (auto it = m_box_map.find(k); it != m_box_map.end())
    {
        if (auto&& v = it->second)
            return v;
    }

    return m_box_map[k] = nullptr;
}

void BoxVisitor::setBoxSlot(box_ptr const& ptr)
{
    if (auto it = m_box_map.find(ptr->name); it != m_box_map.end())
    {
        auto&& k = it->first;
        auto&& v = it->second;

        if (v && v != ptr)
            v->name.clear();

        v = ptr;
    }

}

void BoxVisitor::resetBoxSlot(box_ptr const& ptr)
{
    if (auto it = m_box_map.find(ptr->name); it != m_box_map.end())
    {
        auto&& v = it->second;
        v = nullptr;
        ptr->name.clear();
    }
}

void BoxVisitor::resetAllSlots()
{
    for (auto&& it : m_box_map)
        it.second = nullptr;
}

void BoxVisitor::setBoxSlotAll(box_ptr const& ptr)
{
    setBoxSlot(ptr);
    for (auto&& it : ptr->child_boxes)
        setBoxSlotAll(it);
}