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
                REMOVE, MOVE_INC, MOVE_DEC
            } type;

            box_ptr box;
        };

        Flex            flex;

        box_ptr         parent;
        boxlist_t       child_boxes;



        ut::rect        bounds_inner;
        ut::rect        bounds_outer;

        std::string     name;

        Sizer           sizer;




        static box_ptr create(box_ptr const& parent);

        box_ptr  ptr();
        box_cptr ptr() const;

        box_ptr  deepCopy() const;

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();

        void reset();

        void layout(ut::rect const& b);


        void drawProperties  (BoxVisitor& v);
        void drawTreeTableRow(BoxVisitor& v);
        bool drawTreeTableRow(BoxVisitor& v, bool is_leaf);

        void drawOverlayOutlines     (BoxVisitor& v);
        void drawOverlaySelectedAbove(BoxVisitor& v);
        void drawOverlaySelectedBelow(BoxVisitor& v);

        void applyChildActions();
        void setChildAction(ChildAction const& child_action);

        std::string toYamlString();
        std::string toCPPString();

        bool loadYaml(ut::cstrparam filename);
        bool saveYaml(ut::cstrparam filename);

        static box_ptr createRoot(ut::rect const& bounds);
        static box_ptr createRoot(Sizer const& sizer);

    private:

        bool            highlighted = false;
        ut::color       color       = nextColor();

        std::optional<ChildAction> m_child_action;

        explicit Box(box_ptr p);

        void layoutVbox(ut::rect const& b);
        void layoutHbox(ut::rect const& b);
        void layoutSbox(ut::rect const& b);

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