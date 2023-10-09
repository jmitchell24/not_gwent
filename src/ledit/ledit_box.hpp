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

        ut::rect        bounds_outer;
        ut::rect        bounds_inner;

        std::string     name;

        Sizer           sizer;
        float           inner_pad=10;

        static box_ptr  root_box;
        static box_ptr  selected_box;

        static TreeTableOptions tree_table_options;

        static box_ptr create(box_ptr const& parent);

        box_ptr ptr();

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();
        std::string getPath();

        void reset();

        void layout    (ut::rect const& b);
        void layoutVbox(ut::rect const& b);
        void layoutHbox(ut::rect const& b);
        void layoutSbox(ut::rect const& b);

        void drawProperties();
        void drawTreeTableRow();
        bool drawTreeTableRow(bool is_leaf);
        void drawRect(box_ptr box);

        static void loadYaml(ut::cstrparam filename);
        static void saveYaml(ut::cstrparam filename);



    private:
        struct RowAction
        {
            enum Type { REMOVE, MOVE_INC, MOVE_DEC } type;
            box_ptr box;
        };

        struct PathString
        {
            std::string s; ut::color c;
        };

        std::optional<RowAction> m_row_action;

        Box(box_ptr p);

        void applyRowAction();
        void setRowAction(RowAction const& ra);

        static box_ptr createRoot();

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
    };
}