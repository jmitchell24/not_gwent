#pragma once

//
// Created by james on 8/16/23.
//

#include "record.hpp"

#include "game/card_mover.hpp"
#include "game/layout/row_layout.hpp"

#define ENUM_GAME_CARD_TANK_TARGETS \
    CASE(cpu_hand           , CPU_HAND          ) \
    CASE(cpu_deck           , CPU_DECK          ) \
    CASE(cpu_yard           , CPU_YARD          ) \
    CASE(cpu_special_melee  , CPU_SPECIAL_MELEE ) \
    CASE(cpu_special_ranged , CPU_SPECIAL_RANGED) \
    CASE(cpu_special_siege  , CPU_SPECIAL_SIEGE ) \
    CASE(cpu_combat_melee   , CPU_COMBAT_MELEE  ) \
    CASE(cpu_combat_ranged  , CPU_COMBAT_RANGED ) \
    CASE(cpu_combat_siege   , CPU_COMBAT_SIEGE  ) \
    CASE(usr_hand           , USR_HAND          ) \
    CASE(usr_deck           , USR_DECK          ) \
    CASE(usr_yard           , USR_YARD          ) \
    CASE(usr_special_melee  , USR_SPECIAL_MELEE ) \
    CASE(usr_special_ranged , USR_SPECIAL_RANGED) \
    CASE(usr_special_siege  , USR_SPECIAL_SIEGE ) \
    CASE(usr_combat_melee   , USR_COMBAT_MELEE  ) \
    CASE(usr_combat_ranged  , USR_COMBAT_RANGED ) \
    CASE(usr_combat_siege   , USR_COMBAT_SIEGE  )

namespace game
{
//    enum CardTankTarget
//    {
//#define CASE(x_, y_) TARGET_##y_,
//ENUM_GAME_CARD_TANK_TARGETS
//#undef CASE
//        TARGET_ENUM_COUNT_
//    };


    //TODO: Animation
    // - Use a single spring to control both position / elevation
    // - maintain an optional curve for automatic movement

    using cardid_t      = Card::ID;
    using cardlist_t    = std::vector<Card>;
    using cardidlist_t  = std::vector<cardid_t >;
    using cardidmap_t   = std::unordered_map<cardid_t::value_type, size_t>;

    class CardTank
    {
    public:
        Card&   addCard     (Card card);
        Card&   getCard     (cardid_t id);
        void    removeCard  (cardid_t id);
        void    elevateCard (cardid_t id);

        void update(float dt);
        void draw();
        void drawDebug();

        Card&           addTestCard();
        cardidlist_t    addTestCards(size_t n);

    private:
        size_t              m_next_order_value  = 0;
        cardid_t            m_next_id_value     = cardid_t{1};

        cardlist_t          m_cards;
        cardidmap_t         m_map;
        cardidlist_t        m_old_ids;

        size_t      indexFromId(cardid_t id);
        size_t      nextOrderValue();
        cardid_t    nextIdValue();
        void        rebuildMap();
        void        reorderCards();
    };
}



































