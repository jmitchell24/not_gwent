//
// Created by james on 3/4/24.
//

#pragma once

#include "game/game_params.hpp"
#include "game/game_enums.hpp"

//
// ut
//
#include <ut/math.hpp>

//
// std
//

#include <cstddef>
#include <variant>

#define GAME_EXPAND_ACTION(CASE)    \
    CASE(ActionNop)                 \
    CASE(ActionMoveCard)            \
    CASE(ActionScorch)              \
    CASE(ActionUnitListBuff)        \
    CASE(ActionUnitListNerf)

#define GAME_CASE_DECL_ACTION(x_)           struct x_;
#define GAME_CASE_VARIANT_ACTION(x_)        ,x_
#define GAME_CASE_OVERLOAD_ACTION(x_)       void operator()(x_ const&);

namespace game
{
    //
    // Declarations
    //

    GAME_EXPAND_ACTION(GAME_CASE_DECL_ACTION)

    using Action = std::variant<std::monostate GAME_EXPAND_ACTION(GAME_CASE_VARIANT_ACTION)>;

    using action_param = Action const&;
    using actionlist_t = std::vector<Action>;


    //
    // Actions
    //

    struct ActionNop {};

    struct ActionMoveCard
    {
        PlayerType      from_player;
        PlayerType      to_player;
        size_t          from_idx;
        size_t          to_idx;
        CardListType    list_type;
    };

    struct ActionScorch { };
    struct ActionUnitListBuff { };
    struct ActionUnitListNerf { };
}

namespace game::visitors
{
    struct ExecuteAction
    {
        GameParams& g;

        GAME_EXPAND_ACTION(GAME_CASE_OVERLOAD_ACTION)
    };
}