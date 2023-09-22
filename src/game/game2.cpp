//
// Created by james on 9/15/23.
//

#include <ut/random.hpp>
#include "game/game2.hpp"
using namespace game;

#include "ng/ng_card_data.hpp"

#include "rlImGui/imgui/imgui_extra.hpp"

#include "game/assets.hpp"

#include "gfx/gfx_draw.hpp"
using namespace gfx;

#include <ut/random.hpp>
using namespace ut;

//
// CardStack2
//

void CardStack2::setTestCards(size_t n)
{
    card_ngs.clear();
    for (size_t i = 0; i < n; ++i)
    {
        auto id = (ng::CardID)RNG.nexti((int)ng::CardID::CARD_COUNT_);
        card_ngs.push_back(ng::getCard(id));
    }

    spinner.anim(card_ngs.size());
}

void CardStack2::push(ng::Card const& card)
{
    card_ngs.push_back(card);
    spinner.anim(card_ngs.size());
}

ng::Card CardStack2::pop()
{
    assert(!card_ngs.empty());

    auto card = card_ngs.back();
    card_ngs.pop_back();
    spinner.anim(card_ngs.size());
    return card;
}

void CardStack2::layout(Texture2D t, ut::rect const& b)
{
    card_back = t;
    bounds = layout::CardLayout::fromRect(b).getRect();
    spinner.layout(b.anchorBCtoBC(b.size()/3));





}

void CardStack2::update(float dt)
{
    spinner.update(dt);

    cardrefs_t tmp;
    tmp.reserve(taken_cards.size());

    for (auto&& it: taken_cards)
    {
        assert(it);
        if (it->isNotMoving())
            TANK.removeCard(it.id);
        else
            tmp.push_back(it);
    }

    taken_cards.swap(tmp);
}

void CardStack2::draw()
{
    auto r = bounds;
    auto t = card_back;
    auto o = 2.5f;
    gfx::drawTexture(t, r.withOffset({ o, o}));
    gfx::drawTexture(t, r.withOffset({-o,-o}));
    gfx::drawTexture(t, r);

    gfx::drawRectangleGradientEx(r,
                                 ut::colors::transparent,
                                 ut::colors::black.withNormalA(0.5f),
                                 ut::colors::black.withNormalA(0.5f),
                                 ut::colors::transparent);

    spinner.draw();
}

//
// CardRow2
//


void CardRow2::layout(ut::rect const& b)
{
    bounds = b;
    rebuildLayout();
}

void CardRow2::addCard(size_t idx, CardRef ref)
{
    assert(idx <= cards.size());
    assert(!containsID(ref));

    cards.insert(cards.begin()+ssize_t(idx), ref);
    rebuildLayout();
    arrangeRow();
}

void CardRow2::removeCard(size_t idx)
{
    assert(idx < cards.size());

    cards.erase(cards.begin() + ssize_t(idx));
    rebuildLayout();
    arrangeRow();
}

void CardRow2::removeCard(CardRef ref)
{
    ssize_t idx = getIdx(ref);
    assert(idx >= 0);
    removeCard(size_t(idx));
}

ssize_t CardRow2::getIdx(CardRef ref)
{
    for (size_t i = 0; i < cards.size(); ++i)
        if (cards[i] == ref)
            return i;
    return -1;
}

void CardRow2::rebuildLayout()
{
    layout_row = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER,
            bounds,
            layout::CardLayout::widthFromHeight(bounds.height()),
            cards.size());

    layout_row_next = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER, bounds,
            layout::CardLayout::widthFromHeight(bounds.height()),
            cards.size()+1);
}

//
// GameBoard2
//

void GameBoard2::layout(ut::rect const& bounds)
{
    gb.layout(bounds);
    usr.layout(gb.usr.player,
               gb.usr.melee,
               gb.usr.ranged,
               gb.usr.siege);

    usr.deck.setTestCards(50);
}

void GameBoard2::update(float dt)
{
    usr.update(dt);


}

void GameBoard2::draw()
{
    usr.draw();
}

//Card& GameBoard2::spawnCard(ut::rect const& b, ng::Card const& ng_card)
//{
//    Card c;
//    c.layout = layout::CardLayout::fromRect(b);
//    c.assets = Card::Assets::fromNgCard(ng_card);
//    return TANK.addCard(c);
//}

//void GameBoard2::deSpawnCard(Card& card)
//{
//
//}

void GameBoard2::moveToRow(Card& card, CardRow2& row)
{
    row.addCard(0, card.ref());
    //arrangeRow(row);
}

//void GameBoard2::arrangeRow(CardRow2& row)
//{
//    for (size_t i = 0; i < row.cards.size(); ++i)
//    {
//        row.cards[i]->move2(row.layout_row.getPos(i));
//    }
//}

void GameBoard2::drawCard(Player& p)
{
    if (p.deck.empty())
        return;

    CardRef card = p.deck.giveTopCard();
    p.hand.addCard(0, card);
}



void GameBoard2::discardCard(Player& p)
{
    if (p.hand.empty())
        return;

    CardRef card = p.hand.giveCard(RNG.nexti(p.hand.numCards()));
    p.graveyard.takeCard(card);
}

void GameBoard2::drawDebug()
{
    if (ImGui::Button("Draw"))
    {
        drawCard(usr);
    }

    ImGui::SameLine();

    if (ImGui::Button("Discard"))
    {
        discardCard(usr);
    }

    ImGui::SameLine();

    if (ImGui::Button("Swap"))
    {
        if (usr.siege.special.hasCard())
        {
            CardRef ref = usr.siege.special.giveCard();
            usr.hand.addCard(0, ref);
        }
        else if (!usr.hand.empty())
        {
            CardRef ref = usr.hand.giveCard(usr.hand.numCards()-1);
            usr.siege.special.takeCard(ref);
        }
    }

    gb.drawDebug();

}