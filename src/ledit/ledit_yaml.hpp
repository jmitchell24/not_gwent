//
// Created by james on 10/8/23.
//

#pragma once

#include "ledit/ledit_types.hpp"
#include "ledit/ledit_options.hpp"

//
// yaml-cpp
//
#include <yaml-cpp/yaml.h>

namespace ledit
{

    void emitYaml(YAML::Emitter& emitter, box_ptr const& box);
    void emitYaml(YAML::Emitter& emitter, BoxEditOptions const& opts);
    void emitYaml(YAML::Emitter& emitter, OverlayOptions const& opts);

    void fromYaml(YAML::Node const& node, box_ptr const& box);
    void fromYaml(YAML::Node const& node, BoxEditOptions& opts);
    void fromYaml(YAML::Node const& node, OverlayOptions& opts);
}