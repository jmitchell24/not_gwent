//
// Created by james on 9/29/23.
//

#pragma once

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
    class GameBoard2;

    struct CastUnit;
    struct CastSpy;
    struct CastRowBuff;
    struct CastRowNerf;
    struct CastScorch;
    struct CastLeaderAbility;

    using Cast = std::variant<
        CastUnit,
        CastSpy,
        CastRowBuff,
        CastRowNerf,
        CastScorch,
        CastLeaderAbility
    >;

    //
    // Casts
    //

    struct CastUnit
    {
        enum Row { MELEE, RANGED, SIEGE } row;
        size_t hand_idx;
    };

    struct CastSpy
    {

    };

    struct CastRowBuff
    {
        enum Row { MELEE, RANGED, SIEGE } row;
        size_t hand_idx;
    };

    struct CastRowNerf
    {
        bool change_melee;      /// will melee (both players) be affected?
        bool change_ranged;     /// will ranged (both players) be affected?
        bool change_siege;      /// will siege (both players) be affected?
        bool has_nerf_value;    /// will the nerf be turned on or off?
        size_t hand_idx;
    };

    struct CastScorch
    {

    };

    struct CastLeaderAbility
    {

    };
}

namespace game::visitors
{
    struct CardCaster
    {
        GameBoard2& gb;

        Player& player;
        Player& opponent;

        void operator() (CastUnit const& c);
        void operator() (CastSpy const& c);
        void operator() (CastRowBuff const& c);
        void operator() (CastRowNerf const& c);
        void operator() (CastScorch const& c);
        void operator() (CastLeaderAbility const& c);
    };
}