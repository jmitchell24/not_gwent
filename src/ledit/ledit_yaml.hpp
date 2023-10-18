//
// Created by james on 10/8/23.
//

#pragma once

#include "ledit/ledit_layout.hpp"

#include <yaml-cpp/yaml.h>

namespace ledit
{
    void emitYaml(YAML::Emitter& emitter, box_ptr const& box);
    void fromYaml(YAML::Node const& node, box_ptr const& box);

    void emitYaml(YAML::Emitter& emitter, Layout const& layout);
    void fromYaml(YAML::Node const& node, Layout& layout);
}