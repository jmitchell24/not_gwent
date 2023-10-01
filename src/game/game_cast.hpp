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
    struct CastCommanderHorn;
    struct CastScorch;
    struct CastLeaderAbility;
    struct CastWeather;

    using Cast = std::variant<
            CastUnit,
            CastSpy,
            CastCommanderHorn,
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

    struct CastSpy { };
    struct CastCommanderHorn
    {
        enum Row { MELEE, RANGED, SIEGE } row;
        size_t hand_idx;
    };
    struct CastScorch { };
    struct CastLeaderAbility { };
    struct CastWeather { };











}
