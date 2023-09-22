//
// Created by james on 9/15/23.
//

#pragma once

#include "game/layout/board_layout.hpp"
#include "game/layout/row_layout.hpp"
#include "game/layout/card_row.hpp"
#include "game/card_tank.hpp"
#include "game/assets.hpp"

#include "gfx/gfx_spinner.hpp"
#include "gfx/gfx_draw.hpp"

namespace game
{
    class CardSlot2
    {
    public:
        inline bool hasCard()
        {
            return !m_card.isNil();
        }

        void takeCard(CardRef ref)
        {
            assert(m_card.isNil());
            assert(ref.inTank());

            m_card = ref;
            m_card->move2(m_bounds.pos());
        }

        CardRef giveCard()
        {
            CardRef ref = m_card;
            m_card.reset();
            return ref;
        }

        void layout(ut::rect const& b)
        {
            m_bounds = b;
        }

    private:
        ut::rect    m_bounds;
        CardRef     m_card;
    };



    class CardStack2
    {
    public:
        ut::rect        bounds;
        ng::cardlist_t  card_ngs;

        Texture2D       card_back   { textures::card_back_neutral() };
        gfx::Spinner    spinner     { gfx::Spinner::VERT, ut::colors::burlywood };

        cardrefs_t taken_cards;

        void takeCard(CardRef ref)
        {
            assert(!ref.isNil());

            ref->move2(bounds.pos());
            TANK.elevateCard(ref.id);
            taken_cards.push_back(ref);
            push(ref->ng);
        }

        CardRef giveTopCard()
        {
            Card c;
            c.layout = layout::CardLayout::fromRect(bounds);
            c.assets = Card::Assets::fromNgCard(pop());
            return TANK.addCard(c).ref();
        }

        void layout(Texture2D t, ut::rect const& b);
        void update(float dt);
        void draw();


        inline bool empty() { return card_ngs.empty(); }

        void setTestCards(size_t n);
        void clearTestCards() { setTestCards(0); }

        ng::Card pop();
        void push(ng::Card const& card);
    private:



    };

    class CardRow2
    {
    public:
        ut::rect bounds;

        cardrefs_t          cards;
        layout::RowLayout   layout_row;
        layout::RowLayout   layout_row_next;

        inline bool empty()
        {
            return cards.empty();
        }

        inline bool containsID(CardRef ref)
        {
            return getIdx(ref) >= 0;
        }

        inline size_t numCards() const
        {
            return cards.size();
        }

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref)
        {
            if (size_t idx; layout_row.tryGetIndex(mp,idx))
            {
                ref = cards[idx];
                return true;
            }
            return false;
        }

        CardRef giveCard(size_t idx)
        {
            assert(idx < cards.size());
            CardRef card = cards[idx];
            removeCard(idx);
            return card;
        }

        void addCard      (size_t idx, CardRef ref);
        void removeCard   (size_t idx);
        void removeCard   (CardRef ref);


        void layout (ut::rect const& b);
        void update (float dt ) { }
        void draw   () { }

    private:
        ssize_t getIdx(CardRef ref);

        void rebuildLayout();

        void arrangeRow()
        {
            for (size_t i = 0; i < cards.size(); ++i)
            {
                cards[i]->move2(layout_row.getPos(i));
            }
        }
    };

    struct GameBoard2
    {
        struct CombatRow
        {
            gfx::Spinner score;
            CardSlot2 special;
            CardRow2 units;

            void layout(layout::CombatRow const& l)
            {
                special.layout(l.special);
                units.layout(l.units);
                score.layout(l.score);
            }
        };

        struct Player
        {
            CardStack2  deck;
            CardStack2  graveyard;
            CardRow2    hand;

            CombatRow melee;
            CombatRow ranged;
            CombatRow siege;

            void layout(
                    layout::PlayerRow const& row_player,
                    layout::CombatRow const& row_melee,
                    layout::CombatRow const& row_ranged,
                    layout::CombatRow const& row_siege)
            {
                deck.layout(textures::card_back_neutral(), row_player.deck);
                graveyard.layout(textures::card_back_graveyard(), row_player.graveyard);
                hand.layout(row_player.hand);

                melee.layout(row_melee);
                ranged.layout(row_ranged);
                siege.layout(row_siege);
            }

            void update(float dt)
            {
                deck.update(dt);
                graveyard.update(dt);
                hand.update(dt);

            }

            void draw()
            {
                deck.draw();
                graveyard.draw();
                hand.draw();
            }
        } usr;



        CardRef card_hover;

        layout::GameBoard gb;

        void layout(ut::rect const& bounds);
        void update(float dt);
        void draw();
        void drawDebug();

        //
        // actions
        //

//        Card& spawnCard(ut::rect const& b, ng::Card const& ng_card);
//        void deSpawnCard(Card& card);

        void moveToRow(Card& card, CardRow2& row);
        //void arrangeRow(CardRow2& row);

        void drawCard(Player& p);
        void discardCard(Player& p);

        void setHoveredCard(CardRef ref)
        {
            if (!card_hover.isNil())
            {

            }
        }
    };
}