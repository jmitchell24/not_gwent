//
// Created by james on 10/8/23.
//

#include "ledit/ledit_yaml.hpp"
#include "ledit/ledit_box.hpp"
using namespace ledit;


//
// ut
//
#include <ut/check.hpp>
using namespace ut;

//
// std
//
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
    // color
    //

    template<> struct convert<color>
    {
        using var_type = color;
        static Node encode(var_type const& x)
        {
            return Node{to_string(x)};
        }

        static bool decode(Node const& n, var_type& x)
        {
            return color::tryParseRGBA(n.as<string>().c_str(), x);
        }
    };

    inline Emitter& operator<<(Emitter& em, color const& x)
    {
        return em << Value << to_string(x);
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

    inline Emitter& operator<<(Emitter& em, AnchorType at)
    {
        return em << anchor_to_string(at);
    }

#define VAR_ENCODE(a_, b_, c_, d_, e_)  case Sizer::b_: n[d_] = get<Sizer::b_>(x); break;
#define VAR_DECODE(a_, b_, c_, d_, e_)  if (auto np = n[d_]) { x.emplace<Sizer::b_>(np.as<e_>()); return true; }
#define VAR_EMIT(a_, b_, c_, d_, e_)    case Sizer::b_: em << Flow << BeginMap << Key << (d_) << Value << get<Sizer::b_>(x) << EndMap; break;

    //
    // pad_type
    //

    template<> struct convert<Sizer::pad_type>
    {
        using var_type = Sizer::pad_type;
        static Node encode(var_type const& x)
        {
            Node n;
            switch (x.index())
            {
                EXPAND_PAD(VAR_ENCODE)
                default:nopath("invalid var");
            }
            return n;
        }

        static bool decode(Node const& n, var_type& x)
        {
            if (n.IsMap())
            {
                if (n.size() == 0) { x.emplace<0>(); return true; }
                if (n.size() == 1) { EXPAND_PAD(VAR_DECODE) }
            }
            return false;
        }
    };

    inline Emitter& operator<<(Emitter& em, Sizer::pad_type const& x)
    {
        switch (x.index())
        {
            case 0: break;
            EXPAND_PAD(VAR_EMIT)
            default:nopath("invalid var");
        }
        return em;
    }

    //
    // dim_type
    //

    template<> struct convert<Sizer::dim_type>
    {
        using var_type = Sizer::dim_type;
        static Node encode(var_type const& x)
        {
            Node n;
            switch (x.index())
            {
                EXPAND_DIM(VAR_ENCODE)
                default:nopath("invalid var");
            }
            return n;
        }

        static bool decode(Node const& n, var_type& x)
        {
            if (n.IsMap())
            {
                if (n.size() == 0) { x.emplace<0>(); return true; }
                if (n.size() == 1) { EXPAND_DIM(VAR_DECODE) }
            }
            return false;
        }
    };

    inline Emitter& operator<<(Emitter& em, Sizer::dim_type const& x)
    {
        switch (x.index())
        {
            case 0: break;
            EXPAND_DIM(VAR_EMIT)
            default:nopath("invalid var");
        }
        return em;
    }

    //
    // pos_type
    //

    template<> struct convert<Sizer::pos_type>
    {
        using var_type = Sizer::pos_type;
        static Node encode(var_type const& x)
        {
            Node n;
            switch (x.index())
            {
                EXPAND_POS(VAR_ENCODE)
                default:nopath("invalid var");
            }
            return n;
        }

        static bool decode(Node const& n, var_type& x)
        {
            if (n.IsMap())
            {
                if (n.size() == 0) { x.emplace<0>(); return true; }
                if (n.size() == 1) { EXPAND_POS(VAR_DECODE) }
            }
            return false;
        }
    };

    inline Emitter& operator<<(Emitter& em, Sizer::pos_type const& x)
    {
        switch (x.index())
        {
            case 0: break;
            EXPAND_POS(VAR_EMIT)
            default:nopath("invalid var");
        }
        return em;
    }

    //
    // style
    //

    template<> struct convert<OverlayOptions::Style>
    {
        using var_type = OverlayOptions::Style;
        static Node encode(var_type const& x)
        {
            Node n;
            n["background"] = x.background;
            n["border"] = x.border;
            return n;
        }

        static bool decode(Node const& n, var_type& x)
        {
            if (!n.IsMap())
                return false;

            if (auto np = n["background"])  x.background    = np.as<color>(); else return false;
            if (auto np = n["border"])      x.border        = np.as<color>(); else return false;

            return true;
        }
    };

    inline Emitter& operator<<(Emitter& em, OverlayOptions::Style const& x)
    {
        em << Flow << BeginMap;
        em << Key << "background" << Value << x.background;
        em << Key << "border" << Value << x.border;
        em << EndMap;
        return em;
    }


#undef VAR_ENCODE
#undef VAR_DECODE
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
        em << Key << "type" << Value << box_to_string(box->flex.type);
    }

    LOAD_FUNC(type, n, box)
    {
        if (BoxType t; string_to_box(n["type"].as<string>(), t))
            box->flex.type = t;
        else
            throw Exception(n.Mark(), "invalid box type");
    }

    SAVE_FUNC(inner_pad, em, box)
    {
        if (box->flex.inner_pad != 10)
            em << Key << "inner_pad" << Value << box->flex.inner_pad;
    }

    LOAD_FUNC(inner_pad, n, box)
    {
        box->flex.inner_pad = n["inner_pad"].as<float>(10);
    }

    SAVE_FUNC(weight, em, box)
    {
        if (box->weight != 1.0f)
            em << Key << "weight" << Value << box->weight;
    }

    LOAD_FUNC(weight, n, box)
    {
        box->weight = n["weight"].as<float>(1);
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

    SAVE_FUNC(mrg, em, box) { if (box->sizer.mrg.index() > 0) em << Key << "mrg" << box->sizer.mrg; }
    LOAD_FUNC(mrg, n , box) { if (auto np = n["mrg"]) box->sizer.mrg = np.as<Sizer::mrg_type>(); }

    SAVE_FUNC(pad, em, box) { if (box->sizer.pad.index() > 0) em << Key << "pad" << box->sizer.pad; }
    LOAD_FUNC(pad, n , box) { if (auto np = n["pad"]) box->sizer.pad = np.as<Sizer::pad_type>(); }

    SAVE_FUNC(dim, em, box) { if (box->sizer.dim.index() > 0) em << Key << "dim" << box->sizer.dim; }
    LOAD_FUNC(dim, n , box) { if (auto np = n["dim"]) box->sizer.dim = np.as<Sizer::dim_type>(); }

    SAVE_FUNC(pos, em, box) { if (box->sizer.pos.index() > 0) em << Key << "pos" << box->sizer.pos; }
    LOAD_FUNC(pos, n , box) { if (auto np = n["pos"]) box->sizer.pos = np.as<Sizer::pos_type>(); }

#undef LOAD_FUNC
#undef SAVE_FUNC
}

void ::ledit::emitYaml(YAML::Emitter& em, box_ptr const& box)
{
    using namespace YAML;

    em << BeginMap;
    box_props::type     (em, box);
    box_props::inner_pad(em, box);
    box_props::weight   (em, box);
    box_props::name     (em, box);
    box_props::mrg      (em, box);
    box_props::pad      (em, box);
    box_props::dim      (em, box);
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

    box_props::type     (node, box);
    box_props::weight   (node, box);
    box_props::inner_pad(node, box);
    box_props::name     (node, box);
    box_props::mrg      (node, box);
    box_props::pad      (node, box);
    box_props::dim      (node, box);
    box_props::pos      (node, box);

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

void ::ledit::emitYaml(YAML::Emitter& em, BoxEditOptions const& opts)
{
    using namespace YAML;

#define SAVE(x_) em << Key << #x_ << Value << opts.x_;
    SAVE(show_row_add)
    SAVE(show_row_delete)
    SAVE(show_row_move)
    SAVE(show_row_rename)
    SAVE(show_row_weight)
    SAVE(show_row_type)
#undef SAVE
}

void ::ledit::fromYaml(YAML::Node const& node, BoxEditOptions& opts)
{
    using namespace YAML;

#define LOAD(x_) if (auto np = node[#x_]) { opts.x_ = np.as<bool>(false); }
    LOAD(show_row_add)
    LOAD(show_row_delete)
    LOAD(show_row_move)
    LOAD(show_row_rename)
    LOAD(show_row_weight)
    LOAD(show_row_type)
#undef LOAD
}

void ::ledit::emitYaml(YAML::Emitter& em, OverlayOptions const& opts)
{
    using namespace YAML;

    em << Key << "style_preset_default" << Value << opts.styles[0];
    em << Key << "style_preset_opaque"  << Value << opts.styles[1];
    em << Key << "style_preset_fade"    << Value << opts.styles[2];
    em << Key << "style_preset_hide"    << Value << opts.styles[3];
    em << Key << "style_index"          << Value << opts.style_index;
}

void ::ledit::fromYaml(YAML::Node const& node, OverlayOptions& opts)
{
    using namespace YAML;

    if (auto np = node["style_preset_default"]) { opts.styles[0] = np.as<OverlayOptions::Style>(OverlayOptions::STYLE_PRESET_DEFAULT); }
    if (auto np = node["style_preset_opaque"])  { opts.styles[1] = np.as<OverlayOptions::Style>(OverlayOptions::STYLE_PRESET_OPAQUE); }
    if (auto np = node["style_preset_fade"])    { opts.styles[2] = np.as<OverlayOptions::Style>(OverlayOptions::STYLE_PRESET_FADE); }
    if (auto np = node["style_preset_hide"])    { opts.styles[3] = np.as<OverlayOptions::Style>(OverlayOptions::STYLE_PRESET_HIDE); }
    if (auto np = node["style_index"])          { opts.style_index = clamp(np.as<int>(2), 0, 3); }

}