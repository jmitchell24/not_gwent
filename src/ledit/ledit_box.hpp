//
// Created by james on 10/6/23.
//

#pragma once

#include "ledit_sizer.hpp"
#include "ledit_flex.hpp"

//
// gfx
//
#include "gfx/gfx_view_transform.hpp"

//
// ut
//
#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>
#include <ut/func/delegate.hpp>

//
// std
//
#include <array>
#include <utility>
#include <vector>
#include <string>
#include <optional>

namespace ledit
{
    class Box;
    using box_ptr   = std::shared_ptr<Box>;
    using box_cptr  = std::shared_ptr<Box const>;
    using boxmap_t  = std::unordered_map<std::string, box_ptr>;
    using boxlist_t = std::vector<box_ptr>;

    using rectget_t  = std::optional<ut::rect>;

    struct BoxEditOptions
    {
        bool show_row_select    = true;
        bool show_row_add       = false;
        bool show_row_delete    = false;
        bool show_row_move      = false;
        bool show_row_rename    = false;
        bool show_row_weight    = false;
        bool show_row_type      = false;
    };

    struct OverlayOptions
    {
        ut::color   background {15 ,15 ,15 ,240};
        ut::color   border     {110,110,128,128};

    };



    class BoxVisitor
    {
    public:
        BoxEditOptions      edit_opts;
        OverlayOptions      overlay_opts;
        box_ptr             selected_box;

        std::optional<gfx::ViewTransform> view_transform;

        ut::rect getRect(ut::rect const& p) const;
        ut::vec2 getMousePos(ut::vec2 const& p) const;


        box_ptr     getBox      (ut::cstrparam s);
        void        setBox      (box_ptr const& ptr);
        void        clearBox    (box_ptr const& ptr);

        inline boxmap_t const& boxmap() const { return m_boxmap; }

    protected:
        boxmap_t m_boxmap;
    };

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

        box_ptr         parent;
        boxlist_t       child_boxes;

        BoxType         type        = BOX_HBOX;
        bool            highlighted = false;
        ut::color       color       = nextColor();
        int             weight      = 1;

        ut::rect        bounds_inner;
        ut::rect        bounds_outer;

        std::string     name;

        Sizer           sizer;
        float           inner_pad=10;




        static box_ptr create(box_ptr const& parent);

        box_ptr  ptr();
        box_cptr ptr() const;

        box_ptr  deepCopy() const;

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();

        void reset();

        void layout    (ut::rect const& b);
        void layoutVbox(ut::rect const& b);
        void layoutHbox(ut::rect const& b);
        void layoutSbox(ut::rect const& b);

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
        std::optional<ChildAction> m_child_action;

        explicit Box(box_ptr p);

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

            auto r = child_boxes[0]->weight;
            for (size_t i = 1; i < child_boxes.size(); ++i)
                r = reducer(r, child_boxes[i]->weight);
            return r;
        }

        static ut::color nextColor();

//        bool isSelected();
    };



}