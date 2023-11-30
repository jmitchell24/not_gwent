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



    struct CastUnit;
    struct CastSpy;
    struct CastRowBuff;
    struct CastRowNerf;
    struct CastScorch;
    struct CastLeaderAbility;
    struct CastWeather;

    using Cast = std::variant<
            CastUnit,
            CastSpy,
            CastRowBuff,
            CastRowNerf,
            CastScorch,
            CastLeaderAbility,
            CastWeather
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

    struct CastWeather
    {

    };












}
