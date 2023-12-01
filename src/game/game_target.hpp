//
// Created by james on 9/29/23.
//

#pragma once

#include "game_cast.hpp"

//
// ut
//
#include <ut/math.hpp>

//
// std
//

#include <cstddef>
#include <variant>

namespace game
{
    //
    // Declarations
    //

    class Player;

    struct TargetDefault;
    struct TargetBuff;
    struct TargetNerf;
    struct TargetUnitRow;
    struct TargetUnitCard;

    using Target = std::variant<
        TargetDefault,
        TargetBuff,
        TargetNerf,
        TargetUnitRow,
        TargetUnitCard
    >;

    //
    // Targets
    //

    struct TargetDefault
    {

    };

    struct TargetBuff
    {
        size_t hand_idx;
    };

    struct TargetNerf
    {
        bool target_melee;      /// will melee (both players) be targeted?
        bool target_ranged;     /// will ranged (both players) be targeted?
        bool target_siege;      /// will siege (both players) be targeted?
        bool has_nerf_value;    /// will the nerf be turned on or off?
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

namespace game::visitors
{
    struct ChangeTarget
    {
        Player& player;
        Player& opponent;

        void operator()(TargetDefault const& t);
        void operator()(TargetUnitRow const& t);
        void operator()(TargetUnitCard const& t);
        void operator()(TargetBuff const& t);
        void operator()(TargetNerf const& t);


    };

    struct CastTarget
    {
        Player& player;
        Player& opponent;
        Cast&   cast;

        ut::vec2 const& mp;

        bool operator()(TargetDefault const& t);
        bool operator()(TargetUnitRow const& t) ;
        bool operator()(TargetUnitCard const& t) ;
        bool operator()(TargetBuff const& t);
        bool operator()(TargetNerf const& t);
    };
}