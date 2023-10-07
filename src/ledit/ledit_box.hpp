//
// Created by james on 10/6/23.
//

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
    enum BoxType
    {
        BOX_VBOX, BOX_HBOX, BOX_SBOX
    };

    class Box;
    using box_ptr = std::shared_ptr<Box>;
    using boxlist_t = std::vector<box_ptr>;

    class Box : public std::enable_shared_from_this<Box>
    {
    public:
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

        bool mark = false;

        box_ptr ptr()
        {
            return shared_from_this();
        }

        static box_ptr root_box;
        static box_ptr selected_box;

        struct RowAction
        {
            enum Type { REMOVE, MOVE_UP, MOVE_DOWN } type;
            box_ptr box;
        };

        std::optional<RowAction> row_action;

        void doRowAction()
        {
            if (!row_action)
                return;

            auto ra = *row_action;
            row_action.reset();

            auto b = child_boxes.begin();
            auto e = child_boxes.end();
            for (auto it = b; it != e; ++it)
            {
                if (*it == ra.box)
                {
                    switch (ra.type)
                    {

                        case RowAction::REMOVE:
                            child_boxes.erase(it);
                            return;

                        case RowAction::MOVE_UP:
                            if (it != b)
                                it->swap(*(it-1));
                            return;

                        case RowAction::MOVE_DOWN:
                            if (auto down = it+1; down != e)
                                it->swap(*down);
                            return;
                    }

                    return;
                }
            }
        }

        void rowAction(RowAction const& ra)
        {
            assert(!row_action);
            row_action = ra;
        }

        box_ptr tryGetBox(ut::vec2 const& mp);

        std::string getLbl();

        void layout    (ut::rect const& b);
        void layoutVbox(ut::rect const& b);
        void layoutHbox(ut::rect const& b);
        void layoutSbox(ut::rect const& b);

        void reset();

        void drawPopup();

        void drawTreeTableRow();
        bool drawTreeTableRow(bool is_leaf);
        void drawRect(box_ptr box);

        static box_ptr create(box_ptr const& parent = {})
        {
            return box_ptr { new Box(parent) };
        }

    private:
        Box(box_ptr p)
            : parent{std::move(p)}, color{nextColor()}
        {}

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