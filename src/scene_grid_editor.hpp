//
// Created by james on 10/3/23.
//

#pragma once

#include "scene.hpp"
#include "gfx/gfx_virt2d.hpp"

#include <variant>
#include <optional>

struct RootNode;
struct LeafNode;
struct VboxNode;
struct HboxNode;

//using Node          = std::variant<RootNode, LeafNode, VboxNode, HboxNode>;
//using nodelist_t    = std::vector<Node>;
//
//struct RootNode
//{
//
//};
//
//struct LeafNode { };
//struct VboxNode { };
//struct HboxNode { };




class SceneLayoutEditor : public Scene
{
public:
    char const* name() const override { return "Layout Editor"; }

    ut::rect bounds;

    void layout(ut::rect const& b) override;
    void update(float dt) override;
    void draw() override;
    void drawDebug() override;

private:


    struct GridTag
    {
        enum Type { BOX, VBOX, HBOX, STACKBOX };
        enum AnchorType
        {
            ANCHOR_FILL,


        };

        using text_type     = std::array<char, 20>;
        using taglist_type  = std::vector<GridTag>;

        Type        type        = HBOX;
        bool        highlighted = false;
        ut::color   color       {};
        int         weight      = 1;
        ut::rect    bounds;
        text_type   name;

        float inner_pad =10;
        float outer_pad =10;
        float aspect    =0;

        void normalizeWeights()
        {
            if (child_tags.size() < 2)
                return;

            auto gcd = weightsGCD();

            if (gcd > 1)
            {
                for (auto&& it : child_tags)
                {
                    it.weight /= gcd;
                }
            }

            for (auto&& it : child_tags)
                it.normalizeWeights();
        }

        void layoutVbox(ut::rect const& b)
        {
            auto sz  = child_tags.size();
            auto h   = b.height() - ( outer_pad*2 ) - ( inner_pad * float( sz-1 ) );
            auto sum = weightsSum();
            auto cl  = bounds.min.x + outer_pad;
            auto cr  = bounds.max.x - outer_pad;

            auto cy  = bounds.min.y + outer_pad;

            for (size_t i = 0; i < sz-1; ++i)
            {
                auto&& it   = child_tags[i];
                auto   ch   = h * ( float(it.weight) / sum );
                auto   ct = cy;
                auto   cb = cy+ch;

                it.layout( { cl, ct, cr, cb } );

                cy = cb+inner_pad;
            }

            child_tags.back().layout( { cl, cy, cr, bounds.max.y - outer_pad } );
        }

        void layoutHbox(ut::rect const& b)
        {
            auto sz  = child_tags.size();
            auto w   = b.width() - ( outer_pad*2 ) - ( inner_pad * float( sz-1 ) );
            auto sum = weightsSum();
            auto ct  = bounds.min.y + outer_pad;
            auto cb  = bounds.max.y - outer_pad;

            auto cx  = bounds.min.x + outer_pad;

            for (size_t i = 0; i < sz-1; ++i)
            {
                auto&& it   = child_tags[i];
                auto   cw   = w * ( float(it.weight) / sum );
                auto   cl = cx;
                auto   cr = cx+cw;

                it.layout( { cl, ct, cr, cb } );

                cx = cr+inner_pad;
            }

            child_tags.back().layout( { cx, ct, bounds.max.x - outer_pad, cb } );
        }

        void layoutStackbox(ut::rect const& b)
        {
            for (auto&& it: child_tags)
            {
                it.layout(bounds.shrunk(outer_pad));
            }
        }

        void layoutRect()
        {

        }

        void layout(ut::rect const& b)
        {

            bounds = b;

            if (child_tags.empty())
                return;

            if (child_tags.size() == 1)
            {
                child_tags[0].layout(bounds.shrunk(outer_pad));
                return;
            }

            if (type == VBOX)
            {

            }
            else if (type == HBOX)
            {

            }
            else if (type == STACKBOX)
            {

            }

        }



        taglist_type child_tags;

        GridTag();

        void reset();

        void draw();
        bool draw(bool is_leaf);
        void drawRect();

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
            assert(!child_tags.empty());

            auto r = child_tags[0].weight;
            for (size_t i = 1; i < child_tags.size(); ++i)
                r = reducer(r, child_tags[i].weight);
            return r;
        }
    };

    static GridTag m_tag_root;
    static GridTag m_tag_pending;
};