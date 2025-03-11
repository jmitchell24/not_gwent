//
// Created by james on 12/1/23.
//

#pragma once

#include <ut/check.hpp>

#define GAME_EXPAND_PLAYER_TYPE(CASE) \
    CASE(PLAYER_USR) \
    CASE(PLAYER_CPU)

#define GAME_EXPAND_CARD_LIST(CASE) \
    CASE(CARD_LIST_COMBAT_MELEE     ) \
    CASE(CARD_LIST_COMBAT_RANGED    ) \
    CASE(CARD_LIST_COMBAT_SIEGE     ) \
    CASE(CARD_LIST_DECK             ) \
    CASE(CARD_LIST_YARD             ) \
    CASE(CARD_LIST_HAND             )

#define GAME_CASE_ENUM(x_) ,x_
#define GAME_CASE_STRINGIFY(x_) case x_: return #x_;

namespace game
{
    enum PlayerType { PLAYER_NIL GAME_EXPAND_PLAYER_TYPE(GAME_CASE_ENUM) };
    enum CardListType { CARD_LIST_NIL GAME_EXPAND_CARD_LIST(GAME_CASE_ENUM) };

    enum RowType
    {
        ROW_MELEE,
        ROW_RANGED,
        ROW_SIEGE
    };

    inline char const* stringify(PlayerType x)
    {
        switch (x)
        {
            GAME_EXPAND_PLAYER_TYPE(GAME_CASE_STRINGIFY)

            default: nopath_case(PlayerType);
        }

        return "???";
    }

    inline char const* stringify(CardListType x)
    {
        switch (x)
        {
            GAME_EXPAND_CARD_LIST(GAME_CASE_STRINGIFY)

            default: nopath_case(PlayerType);
        }

        return "???";
    }
}