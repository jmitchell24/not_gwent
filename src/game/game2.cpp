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

void CardStack2::layout(Texture2D t, ut::rect const& b)
{
    card_back = t;
    bounds = layout::CardLayout::fromRect(b).getRect();
    spinner.layout(b.anchorBCtoBC(b.size()/3));


    for (size_t i = 0; i < 50; ++i)
    {
        auto id = (ng::CardID)RNG.nexti((int)ng::CardID::CARD_COUNT_);
        cards.push_back(ng::getCard(id));
    }

    spinner.anim(cards.size());
}

void CardStack2::update(float dt)
{
    spinner.update(dt);
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

void CardRow2::addID(size_t idx, cardid_t id)
{
    assert(idx <= ids.size());
    ids.insert(ids.begin()+ssize_t(idx), id);
    rebuildLayout();
}

void CardRow2::removeID(size_t idx)
{
    assert(idx < ids.size());
    ids.erase(ids.begin() + ssize_t(idx));
    rebuildLayout();
}

void CardRow2::removeID(cardid_t id)
{
    ssize_t idx = getIdx(id);
    assert(idx >= 0);
    removeID(size_t(idx));
}

ssize_t CardRow2::getIdx(cardid_t id)
{
    for (size_t i = 0; i < ids.size(); ++i)
        if (ids[i] == id)
            return i;
    return -1;
}

void CardRow2::rebuildLayout()
{
    layout_row = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER,
            bounds,
            layout::CardLayout::widthFromHeight(bounds.height()),
            ids.size());

    layout_row_next = layout::RowLayout::create(
            layout::RowLayout::DIR_CENTER, bounds,
            layout::CardLayout::widthFromHeight(bounds.height()),
            ids.size()+1);
}

//
// GameBoard2
//

void GameBoard2::layout(ut::rect const& bounds)
{
    gb.layout(bounds);

    row1.layout(gb.player_usr.hand);
    row2.layout(gb.player_cpu.hand);
    spinner.layout(gb.combat_usr_melee.score);

    card_stack_deck.layout(textures::card_back_neutral(), gb.player_usr.deck);
    card_stack_graveyard.layout(textures::card_back_graveyard(), gb.player_usr.graveyard);
}

void GameBoard2::update(float dt)
{
    tank.update(dt);
    spinner.update(dt);

    card_stack_deck.update(dt);
    card_stack_graveyard.update(dt);
}

void GameBoard2::draw()
{
    tank.draw();
    spinner.draw();

    card_stack_graveyard.draw();
    card_stack_deck.draw();
}

void GameBoard2::drawDebug()
{
    if (ImGui::Button("+"))
    {
        auto idx = RNG.nexti(row1.ids.size());
        int score = 0;

        row1.addID(idx, tank.addTestCard().id());

        for (size_t i = 0; i < row1.ids.size(); ++i)
        {
            auto l = row1.layout_row.getCardLayout(i);

            Card& card = tank.getCard(row1.ids[i]);

            card.layout.setSize(l.getSize());
            card.spring.setDst(l.getPosition3());

            if (card.ng.isUnitCard())
                score += card.ng.asUnitCard().strength;
        }

        spinner.anim(score);
    }

    ImGui::SameLine();

    if (row1.ids.empty())
    {
        ImGui::BeginDisabled();
        ImGui::Button("-");
        ImGui::EndDisabled();
    }
    else if (ImGui::Button("-"))
    {
        auto idx = RNG.nexti(row1.ids.size());

        tank.removeCard(row1.ids[idx]);
        row1.removeID(idx);

        int score = 0;

        for (size_t i = 0; i < row1.ids.size(); ++i)
        {
            auto l = row1.layout_row.getCardLayout(i);

            Card& card = tank.getCard(row1.ids[i]);

            card.layout.setSize(l.getSize());
            card.spring.setDst(l.getPosition3());

            if (card.ng.isUnitCard())
                score += card.ng.asUnitCard().strength;
        }

        spinner.anim(score);
    }

    gb.drawDebug();
    tank.drawDebug();

}