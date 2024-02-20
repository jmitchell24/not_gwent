//
// Created by james on 2/19/24.
//

#pragma once

#include <variant>

namespace game
{
    class Player;

    struct AbilityScorch;
    struct AbilityMedic;
    struct AbilityMuster;

    using Ability = std::variant<
            AbilityScorch,
            AbilityMedic,
            AbilityMuster

            >;

    struct AbilityParams
    {
        Player& player;
        Player& opponent;
    };
}

namespace game::visitors
{
    struct TriggerAbility
    {
        AbilityParams params;

        void operator() (AbilityScorch const& a);
        void operator() (AbilityMedic const& a);
        void operator() (AbilityMuster const& a);
    };


}