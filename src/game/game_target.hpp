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
    struct TargetCastUnit;
    struct TargetScorch;
    struct TargetDecoy;
    struct TargetMedic;

    using Target = std::variant<
        TargetDefault,
        TargetBuff,
        TargetNerf,
        TargetCastUnit,
        TargetScorch,
        TargetDecoy,
        TargetMedic
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

    struct TargetCastUnit
    {
        bool target_melee;
        bool target_ranged;
        bool target_siege;
        bool target_opponent;
        size_t hand_idx;
    };

    struct TargetScorch
    {
        size_t hand_idx;
    };

    struct TargetDecoy
    {
        size_t hand_idx;
    };

    struct TargetMedic
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
        void operator()(TargetCastUnit const& t);
        void operator()(TargetScorch const& t);
        void operator()(TargetDecoy const& t);
        void operator()(TargetBuff const& t);
        void operator()(TargetNerf const& t);
        void operator()(TargetMedic const& t);

    };

    struct CastTarget
    {
        Player& player;
        Player& opponent;
        Cast&   cast;

        ut::vec2 const& mp;

        bool operator()(TargetDefault const& t);
        bool operator()(TargetCastUnit const& t) ;
        bool operator()(TargetScorch const& t) ;
        bool operator()(TargetDecoy const& t);
        bool operator()(TargetBuff const& t);
        bool operator()(TargetNerf const& t);
        bool operator()(TargetMedic const& t);
    };
}