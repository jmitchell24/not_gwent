//
// Created by james on 9/29/23.
//

#pragma once

#include "game/game_cast.hpp"
#include "game/game_card_picker.hpp"

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

    using Target = std::variant<
        TargetDefault,
        TargetBuff,
        TargetNerf,
        TargetCastUnit,
        TargetScorch,
        TargetDecoy
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
        bool is_medic;
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

    struct CastTargetParams
    {
        Player&             player;
        Player&             opponent;
        CardPicker&         card_picker;
        board::BoardBoss&   boss;
    };
}

namespace game::visitors
{
    struct ChangeTarget
    {
        CastTargetParams params;

        void operator()(TargetDefault const& t);
        void operator()(TargetCastUnit const& t);
        void operator()(TargetScorch const& t);
        void operator()(TargetDecoy const& t);
        void operator()(TargetBuff const& t);
        void operator()(TargetNerf const& t);
    };

    struct CastTarget
    {
        CastTargetParams    params;
        Cast&               cast;
        ut::vec2 const&     mp;


        bool operator()(TargetDefault const& t);
        bool operator()(TargetCastUnit const& t);
        bool operator()(TargetScorch const& t);
        bool operator()(TargetDecoy const& t);
        bool operator()(TargetBuff const& t);
        bool operator()(TargetNerf const& t);

    private:
        void setCast(Cast const& c);
        void setTarget(Target const& c);
    };
}