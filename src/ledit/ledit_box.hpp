//
// Created by james on 10/6/23.
//

#pragma once

#include "ledit_sizer.hpp"

//
// ut
//
#include <ut/math.hpp>
#include <ut/color.hpp>
#include <ut/string.hpp>

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
    using boxlist_t = std::vector<box_ptr>;

    class Box : public std::enable_shared_from_this<Box>
    {
    public:
        struct TreeTableOptions
        {
            bool show_row_select    =true;
            bool show_row_add       =false;
            bool show_row_delete    =false;
            bool show_row_move      =false;
            bool show_row_rename    =false;
            bool show_row_weight    =false;
            bool show_row_type      =false;
        };

        box_ptr         parent;
        boxlist_t       child_boxes;

        BoxType         type        = BOX_HBOX;
        bool            highlighted = false;
        ut::color       color       = nextColor();
        int             weight      = 1;

        ut::rect        bounds_parent;
        ut::rect        bounds_pad;
        ut::rect        bounds_content;

        std::string     name;

        Sizer           sizer;
        float           inner_pad=10;

        static box_ptr          root_box;
        static box_ptr          selected_box;
        static TreeTableOptions tree_table_options;

        static box_ptr create(box_ptr const& parent);

        box_ptr ptr();

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();

        void reset();

        void layout    (ut::rect const& b);
        void layoutVbox(ut::rect const& b);
        void layoutHbox(ut::rect const& b);
        void layoutSbox(ut::rect const& b);

        void drawProperties();
        void drawTreeTableRow();
        bool drawTreeTableRow(bool is_leaf);


        void drawOverlayOutlines();
        void drawOverlaySelectedAbove();
        void drawOverlaySelectedBelow();


        static bool loadYaml(ut::cstrparam filename);
        static bool saveYaml(ut::cstrparam filename);
        static box_ptr createRoot();

        static void drawOverlay();
        static void drawWindowSelectedBox();
        static void drawWindowBoxHierarchy();


    private:
        struct RowAction
        {
            enum Type { REMOVE, MOVE_INC, MOVE_DEC } type;
            box_ptr box;
        };

        std::optional<RowAction> m_row_action;

        Box(box_ptr p);

        void applyRowAction();
        void setRowAction(RowAction const& ra);

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

        bool isSelected();
    };


}