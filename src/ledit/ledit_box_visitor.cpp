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

//
// Box Slot
//

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

//
// Box Selection
//

void BoxVisitor::setSelectedBoxSingle(box_ptr const& ptr)
{
    m_selected_box_single = ptr;
    m_selected_box_multi.clear();
}

void BoxVisitor::setSelectedBoxMulti(box_ptr const& ptr)
{
    for (auto&& it : m_selected_box_multi)
        if (it == ptr)
            return;
    m_selected_box_multi.push_back(ptr);
}

void BoxVisitor::toggleSelectedBoxMulti(box_ptr const& ptr)
{
    auto b = m_selected_box_multi.begin();
    auto e = m_selected_box_multi.end();
    for (auto it = b; it != e; ++it)
    {
        if (*it == ptr)
        {
            m_selected_box_multi.erase(it);
            return;
        }
    }
    m_selected_box_multi.push_back(ptr);
}

void BoxVisitor::clearSelectedBoxSingle()
{
    m_selected_box_single = nullptr;
    m_selected_box_multi.clear();
}

void BoxVisitor::clearSelectedBoxMulti(box_ptr const& ptr)
{
    auto b = m_selected_box_multi.begin();
    auto e = m_selected_box_multi.end();
    for (auto it = b; it != e; ++it)
    {
        if (*it == ptr)
        {
            m_selected_box_multi.erase(it);
            return;
        }
    }
}
void BoxVisitor::clearSelectedBoxMultiAll()
{
    m_selected_box_multi.clear();
}

bool BoxVisitor::isBoxSelectedSingle(box_ptr const& ptr) const
{
    return m_selected_box_single == ptr;
}

bool BoxVisitor::isBoxSelectedMulti(box_ptr const& ptr) const
{
    for (auto&& it : m_selected_box_multi)
        if (it == ptr)
            return true;
    return false;
}

bool BoxVisitor::hasBoxSelectionSingle() const
{
    return m_selected_box_single != nullptr;
}

bool BoxVisitor::hasBoxSelectionMulti() const
{
    return !m_selected_box_multi.empty();
}

box_ptr BoxVisitor::boxSelectionSingle() const
{
    return m_selected_box_single;
}

boxlist_t BoxVisitor::boxSelectionMulti() const
{
    return m_selected_box_multi;
}

cstrview BoxVisitor::getBoxSelectionLabel(box_ptr const& ptr) const
{
    if (isBoxSelectedSingle(ptr))
        return "o";
    if (isBoxSelectedMulti(ptr))
        return "*";
    return " ";
}