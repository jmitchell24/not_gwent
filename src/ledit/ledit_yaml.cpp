//
// Created by james on 10/8/23.
//

#include "ledit/ledit_yaml.hpp"
using namespace ledit;

#include "assert_msg.hpp"

void ::ledit::emitYaml(YAML::Emitter& emitter, Sizer const& sizer)
{
    using namespace YAML;

    switch (sizer.padType())
    {
        case Sizer::PAD_NONE:
            break;

        case Sizer::PAD_ONE:
            emitter << Key << "pad1" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getPad1() <<
                    EndSeq;
            break;

        case Sizer::PAD_TWO:
            emitter << Key << "pad2" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getPad2().x <<
                    sizer.getPad2().y <<
                    EndSeq;
            break;

        case Sizer::PAD_FOUR:
            emitter << Key << "pad4" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getPad4().x <<
                    sizer.getPad4().y <<
                    sizer.getPad4().z <<
                    sizer.getPad4().w <<
                    EndSeq;
            break;

        default:nopath_case(Sizer::PadType);
    }

    switch (sizer.sclType())
    {
        case Sizer::SCL_NONE:
            break;

        case Sizer::SCL_ASPECT:
            emitter << Key << "sclAspect" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getSclAspect() <<
                    EndSeq;
            break;

        case Sizer::SCL_SCALE:
            emitter << Key << "sclScale" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getSclScale() <<
                    EndSeq;
            break;

        case Sizer::SCL_SCALE_XY:
            emitter << Key << "sclScaleXY" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getSclScaleXY().x <<
                    sizer.getSclScaleXY().y <<
                    EndSeq;
            break;

        default:nopath_case(Sizer::SclType);
    }

    switch (sizer.posType())
    {
        case Sizer::POS_NONE:
            break;

        case Sizer::POS_ANCHOR:
            emitter << Key << "posAnchor" << Value;
            emitter << Flow << BeginSeq <<
                    anchor_to_string(sizer.getPosAnchor()) <<
                    EndSeq;
            break;

        case Sizer::POS_XY:
            emitter << Key << "sclScaleXY" << Value;
            emitter << Flow << BeginSeq <<
                    sizer.getSclScaleXY().x <<
                    sizer.getSclScaleXY().y <<
                    EndSeq;
            break;

        default:nopath_case(Sizer::PosType);
    }
}

void ::ledit::emitYaml(YAML::Emitter& emitter, box_ptr box)
{
    using namespace YAML;

    emitter << BeginMap;
    emitter << Key << "type" << Value << box_to_string(box->type);
    emitter << Key << "weight" << Value << box->weight;

    if (!box->name.empty())
    {
        emitter << Key << "name" << Value << box->name;
    }

    emitYaml(emitter, box->sizer);

    if (!box->child_boxes.empty())
    {
        emitter << Key << "children" << BeginSeq;
        for (auto&& it: box->child_boxes)
        {
            emitYaml(emitter, it);
        }
        emitter << EndSeq;
    }


    emitter << EndMap;
}