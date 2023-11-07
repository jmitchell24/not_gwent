//
// Created by james on 10/6/23.
//

#pragma once

#include "ledit/ledit_sizer.hpp"
#include "ledit/ledit_flex.hpp"
#include "ledit/ledit_box_visitor.hpp"

namespace ledit
{
    class Box : public std::enable_shared_from_this<Box>
    {
    public:
        struct ChildAction
        {
            enum Type
            {
                DELETE, MOVE_INC, MOVE_DEC, CLONE
            } type;

            box_ptr box;
        };

        std::string     name;
        Flex            flex;
        Sizer           sizer;

        ut::rect        bounds_inner;
        ut::rect        bounds_outer;

        box_ptr         parent;
        boxlist_t       child_boxes;

        void setChanged(bool b);
        bool getChanged() const;

        box_ptr  ptr();
        box_cptr ptr() const;

        box_ptr  deepCopy(box_ptr const& parent);

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();

        void reset();

        std::string toYamlString();
        std::string toCPPString();

        bool loadYaml(ut::cstrparam filename);
        bool saveYaml(ut::cstrparam filename);

        static bool isRoot(box_ptr const& box)
        { return box && box->parent == nullptr; }

        static box_ptr create       (box_ptr const& parent);
        static box_ptr createRoot   (ut::rect const& bounds);
        static box_ptr createRoot   (Sizer const& sizer);

        static void drawBoxHierarchy    (BoxVisitor& v);
        static void drawPropertiesTab   (BoxVisitor& v);
        static void drawOverlay         (BoxVisitor& v);



    private:
        ut::color   m_color;
        bool        m_changed;

        std::optional<ChildAction> m_child_action;

        explicit Box(box_ptr p);



        //
        // layout
        //

        void calcLayout    (ut::rect const& b);
        void calcLayoutVbox(ut::rect const& b);
        void calcLayoutHbox(ut::rect const& b);
        void calcLayoutSbox(ut::rect const& b);

        void setBounds(ut::rect const& inner, ut::rect const& outer);

        //
        // child action
        //

        void insertChildEnd(BoxVisitor& v);
        void insertChildStart(BoxVisitor& v);

        void parentActionDelete   (BoxVisitor& v);
        void parentActionClone    (BoxVisitor& v);
        void parentActionMoveInc  (BoxVisitor& v);
        void parentActionMoveDec  (BoxVisitor& v);

        void applyChildActions();

        //
        // draw
        //

        void drawProperties  (BoxVisitor& v);
        void drawTreeTableRow(BoxVisitor& v);
        bool drawTreeTableRow(BoxVisitor& v, bool is_leaf);

        void drawOverlayOutlines     (BoxVisitor& v);
        void drawOverlaySelectedAbove(BoxVisitor& v);
        void drawOverlaySelectedBelow(BoxVisitor& v);

        //
        // helpers
        //

        inline float weightsSum() const
        {
            return (float)reduceWeights([](auto a, auto b) { return a + b; });
        }

        inline int weightsGCD() const
        {
            return reduceWeights([](auto a, auto b)
            {
                while (b != 0)
                {
                    auto tmp = b;
                    b = a % b;
                    a = tmp;
                }
                return a;
            });
        }

        template <typename Reducer> int reduceWeights(Reducer&& reducer) const
        {
            assert(!child_boxes.empty());

            auto r = child_boxes[0]->flex.weight;
            for (size_t i = 1; i < child_boxes.size(); ++i)
                r = reducer(r, child_boxes[i]->flex.weight);
            return r;
        }

        static ut::color nextColor();
    };
}