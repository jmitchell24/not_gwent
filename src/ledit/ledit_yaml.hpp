//
// Created by james on 10/8/23.
//

#pragma once

#include "ledit/ledit_box.hpp"

#include <yaml-cpp/yaml.h>

namespace ledit
{
    void emitYaml(YAML::Emitter& emitter, Sizer const& sizer);
    void emitYaml(YAML::Emitter& emitter, box_ptr const& box);

    void fromYaml(YAML::Node const& node, box_ptr const& box);
    void fromYaml(YAML::Node const& node, Sizer& sizer);
}