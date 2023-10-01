//
// Created by james on 9/29/23.
//

#pragma once

#include <variant>

namespace game
{
    //
    // Declarations
    //

    struct TargetDefault;
    struct TargetCommanderHorn;
    struct TargetUnitRow;
    struct TargetUnitCard;

    using Target = std::variant<
            TargetDefault,
            TargetCommanderHorn,
            TargetUnitRow,
            TargetUnitCard
    >;

    //
    // Targets
    //

    struct TargetDefault
    {

    };

    struct TargetCommanderHorn
    {
        size_t hand_idx;
    };

    struct TargetUnitRow
    {
        bool target_melee;
        bool target_ranged;
        bool target_siege;
        bool target_opponent;
        size_t hand_idx;
    };

    struct TargetUnitCard
    {
        size_t hand_idx;
    };
}