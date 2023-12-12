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

        BoxRects        rects;

        box_ptr         parent;
        boxlist_t       child_boxes;



        void clearWantBind();
        bool wantBind() const;

        box_ptr ptr();
        box_ptr deepCopy(box_ptr const& parent);
        void mutate(box_ptr const& original);
        box_ptr tryGetBox(ut::vec2 const& mp);

        void reset();


        std::string toYamlString();
        bool loadYaml(ut::cstrparam filename);
        bool saveYaml(ut::cstrparam filename);

        void drawBreadcrumbs();

        static bool isRoot(box_ptr const& box);

        static box_ptr create       (box_ptr const& parent);
        static box_ptr createRoot   (ut::rect const& bounds);
        static box_ptr createRoot   (Sizer const& sizer);

        static void drawBoxHierarchy();
        static void drawOverlay();
        static void drawPropertiesWindow();

    private:
        using ca_type = std::optional<ChildAction>;

        ut::color   m_color;
        bool        m_want_bind;
        ca_type     m_child_action;

        explicit Box(box_ptr p);

        //
        // layout
        //

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

        void parentActionDelete     ();
        void parentActionClone      ();
        void parentActionMoveInc    ();
        void parentActionMoveDec    ();

        void applyChildActions();

        //
        // draw
        //

        void drawProperties  ();
        void drawTreeTableRow();
        bool drawTreeTableRow(bool is_leaf);

        void drawOverlayOutlines            ();
        void drawOverlaySingleSelectedAbove ();
        void drawOverlaySingleSelectedBelow ();

        //
        // other
        //

        std::string getLbl() const;
        void normalizeWeights();
        void notifyChanged();
        void setWantBindAll();
    };
}