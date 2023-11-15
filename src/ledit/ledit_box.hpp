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
        float           weight;
        Flex            flex;
        Sizer           sizer;

        ut::rect        bounds_inner;
        ut::rect        bounds_outer;

        box_ptr         parent;
        boxlist_t       child_boxes;

        void setChangedAll();
        void clearChanged();
        bool getChanged() const;

        box_ptr  ptr();
        box_cptr ptr() const;

        box_ptr  deepCopy(box_ptr const& parent);
        void mutate(box_ptr const& original);

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl() const;

        void reset();
        void normalizeWeights();

        std::string toYamlString();

        bool loadYaml(ut::cstrparam filename);
        bool saveYaml(ut::cstrparam filename);

        static bool isRoot(box_ptr const& box)
        { return box && box->parent == nullptr; }

        static box_ptr create       (box_ptr const& parent);
        static box_ptr createRoot   (ut::rect const& bounds);
        static box_ptr createRoot   (Sizer const& sizer);

        static void drawBoxHierarchy    (BoxVisitor& v);
        static void drawOverlay         (BoxVisitor& v);
        static void drawPropertiesWindow(BoxVisitor& v);

    private:
        using ca_type = std::optional<ChildAction>;

        ut::color   m_color;
        bool        m_changed;
        ca_type     m_child_action;

        explicit Box(box_ptr p);

        //
        // layout
        //

        void setBounds(ut::rect const& inner, ut::rect const& outer);

        void calcLayout    (ut::rect const& b);
        void calcLayoutVbox(ut::rect const& b);
        void calcLayoutHbox(ut::rect const& b);
        void calcLayoutSbox(ut::rect const& b);

        //
        // weight calculation
        //

        void calcWeightsPropAnchor(box_ptr const& anchor);
        void calcWeightsUniformAnchor(box_ptr const& anchor);
        void calcWeightsUniform();
        void calcWeightsNormal();

        //
        // child action
        //

        void insertChildEnd();
        void insertChildStart();
        void insertChild(boxlist_t::iterator const& pos, box_ptr const& box);

        void parentActionDelete     (BoxVisitor& v);
        void parentActionClone      (BoxVisitor& v);
        void parentActionMoveInc    (BoxVisitor& v);
        void parentActionMoveDec    (BoxVisitor& v);

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



        static ut::color nextColor();
    };
}