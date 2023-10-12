//
// Created by james on 10/8/23.
//

#include "ledit/ledit_yaml.hpp"
using namespace ledit;

#include "check.hpp"



//
// ut
//
using namespace ut;

//
// std
//
#include <valarray>
using namespace std;

//
// Yaml-CPP conversions
//

namespace YAML
{
    //
    // vecnf<D>
    //
    template<size_t D> struct convert<vecnf<D>>
    {
        static Node encode(vecnf<D> const& v)
        {
            Node n;
            for (auto&& it : v.pack)
                n.push_back(it);
            return n;
        }

        static bool decode(Node const& n, vecnf<D>& v)
        {
            if(!n.IsSequence() || n.size() != D)
                return false;

            for (size_t i = 0; i < D; ++i)
            {
                v.pack[i] = n[i].as<float>();
                v.pack[i] = n[i].as<float>();
                v.pack[i] = n[i].as<float>();
            }
            return true;
        }
    };

    template <size_t D>
    inline Emitter& operator<<(Emitter& em, vecnf<D> const& v)
    {
        em << BeginSeq;
        for (auto&& it : v.pack)
            em << it;
        return em << EndSeq;
    }

    //
    // AnchorType
    //
    template<> struct convert<AnchorType>
    {
        static Node encode(AnchorType at)
        {
            auto s = anchor_to_string(at).str();
            return Node{s};
        }

        static bool decode(Node const& n, AnchorType& at)
        {
            if (!n.IsScalar())
                return false;
            return string_to_anchor(n.as<string>(), at);
        }
    };

    template <size_t D>
    inline Emitter& operator<<(Emitter& em, AnchorType at)
    {
        return em << anchor_to_string(at);
    }
}


//
// Property Save/Load
//

namespace box_props
{
    using namespace YAML;

#define SAVE_FUNC(a_, b_, c_) void a_(Emitter& b_, box_ptr const& c_)
#define LOAD_FUNC(a_, b_, c_) void a_(Node const& b_, box_ptr const& c_)

    SAVE_FUNC(type, em, box)
    {
        em << Key << "type" << Value << box_to_string(box->type);
    }

    LOAD_FUNC(type, n, box)
    {
        if (BoxType t; string_to_box(n["type"].as<string>(), t))
            box->type = t;
        else
            throw Exception(n.Mark(), "invalid box type");
    }

    SAVE_FUNC(weight, em, box)
    {
        if (box->weight != 1)
            em << Key << "weight" << Value << box->weight;
    }

    LOAD_FUNC(weight, n, box)
    {
        box->weight = n["weight"].as<int>(1);
    }

    SAVE_FUNC(name, em, box)
    {
        if (!box->name.empty())
            em << Key << "name" << Value << box->name;
    }

    LOAD_FUNC(name, n, box)
    {
        box->name = n["name"].as<string>("");
    }

    SAVE_FUNC(pad, em, box)
    {
        switch (box->sizer.padType())
        {
            case Sizer::PAD_NONE: break;
            case Sizer::PAD_ONE : em << Key << "pad" << Key << Flow << BeginMap << Key << "pad1" << Value << box->sizer.getPad1() << EndMap; break;
            case Sizer::PAD_TWO : em << Key << "pad" << Key << Flow << BeginMap << Key << "pad2" << Value << box->sizer.getPad2() << EndMap; break;
            case Sizer::PAD_FOUR: em << Key << "pad" << Key << Flow << BeginMap << Key << "pad4" << Value << box->sizer.getPad4() << EndMap; break;

            default:nopath_case(Sizer::PadType);
        }
    }

    LOAD_FUNC(pad, n, box)
    {
        if (auto npad = n["pad"])
        {
                 if (auto npad1 = npad["pad1"]) box->sizer.setPad1(npad1.as<float>());
            else if (auto npad2 = npad["pad2"]) box->sizer.setPad2(npad2.as<vec2 >());
            else if (auto npad4 = npad["pad4"]) box->sizer.setPad4(npad4.as<vec4 >());
            else
                throw Exception(n.Mark(), "invalid pad");
        }
    }

    SAVE_FUNC(scl, em, box)
    {
        switch (box->sizer.sclType())
        {
            case Sizer::SCL_NONE: break;
            case Sizer::SCL_ASPECT  : em << Key << "scl" << Key << Flow << BeginMap << Key << "aspect"  << Value << box->sizer.getSclAspect()  << EndMap; break;
            case Sizer::SCL_SCALE   : em << Key << "scl" << Key << Flow << BeginMap << Key << "scale"   << Value << box->sizer.getSclScale()   << EndMap; break;
            case Sizer::SCL_SCALE_XY: em << Key << "scl" << Key << Flow << BeginMap << Key << "scalexy" << Value << box->sizer.getSclScaleXY() << EndMap; break;

            default:nopath_case(Sizer::SclType);
        }
    }

    LOAD_FUNC(scl, n, box)
    {
        if (auto nscl = n["scl"])
        {
            if (auto naspect  = nscl["aspect" ]) box->sizer.setSclAspect (naspect .as<float>()); else
            if (auto nscale   = nscl["scale"  ]) box->sizer.setSclScale  (nscale  .as<float>()); else
            if (auto nscalexy = nscl["scalexy"]) box->sizer.setSclScaleXY(nscalexy.as<vec2 >()); else
            throw Exception(n.Mark(), "invalid scl");
        }
    }

    SAVE_FUNC(pos, em, box)
    {
        switch (box->sizer.posType())
        {
            case Sizer::POS_NONE   : break;
            case Sizer::POS_XY     : em << Key << "pos" << Key << Flow << BeginMap << Key << "posxy"  << Value << box->sizer.getPosXY()     << EndMap; break;
            case Sizer::POS_ANCHOR : em << Key << "pos" << Key << Flow << BeginMap << Key << "anchor" << Value << anchor_to_string(box->sizer.getPosAnchor()) << EndMap; break;

            default:nopath_case(Sizer::SclType);
        }
    }

    LOAD_FUNC(pos, n, box)
    {
        if (auto npos = n["pos"])
        {
            if (auto nposxy = npos["posxy"])
            {
                box->sizer.setPosXY(nposxy .as<vec2>());
            }
            else if (auto nanchor = npos["anchor"])
            {
                if (AnchorType t; string_to_anchor(nanchor.as<string>(), t))
                    box->sizer.setPosAnchor(t);
                else
                    throw Exception(n.Mark(), "invalid anchor type");
            }
            else
            {
                throw Exception(n.Mark(), "invalid pos");
            }
        }
    }

#undef LOAD_FUNC
#undef SAVE_FUNC
}

void ::ledit::emitYaml(YAML::Emitter& em, box_ptr const& box)
{
    using namespace YAML;

    em << BeginMap;
    box_props::type     (em, box);
    box_props::weight   (em, box);
    box_props::name     (em, box);
    box_props::pad      (em, box);
    box_props::scl      (em, box);
    box_props::pos      (em, box);

    if (!box->child_boxes.empty())
    {
        em << Key << "children" << BeginSeq;
        for (auto&& it: box->child_boxes)
            emitYaml(em, it);
        em << EndSeq;
    }

    em << EndMap;
}

void ::ledit::fromYaml(YAML::Node const& node, box_ptr const& box)
{
    using namespace YAML;

    box_props::type   (node, box);
    box_props::weight (node, box);
    box_props::name   (node, box);
    box_props::pad    (node, box);
    box_props::scl    (node, box);
    box_props::pos    (node, box);

    if (auto ch = node["children"])
    {
        if (ch.IsSequence())
        {
            for (auto&& it : ch)
            {
                auto b = Box::create(box);
                fromYaml(it, b);
                box->child_boxes.push_back(b);
            }
        }
        else
            throw Exception(ch.Mark(), "invalid children seq");
    }
}

