//
// Created by james on 9/15/23.
//

#include <ut/random.hpp>
#include "game/game2.hpp"
using namespace game;

#include "ng/ng_card_data.hpp"

#include "rlImGui/imgui/imgui_mods.hpp"

#include "game/assets.hpp"

#include "gfx/gfx_draw.hpp"
using namespace gfx;

#include <ut/random.hpp>
using namespace ut;

//
// GameBoard2
//

GameBoard2::GameBoard2()
{
    usr.stats.name = "Triss";
    usr.stats.deck_name = "Northern Realms";
    usr.stats.lead_name = "Foltest";
    usr.stats.avatar_texture = textures::avatar_triss();

    cpu.stats.name = "Leshen";
    cpu.stats.deck_name = "Monsters";
    cpu.stats.lead_name = "Eredin";
    cpu.stats.avatar_texture = textures::avatar_leshen();

}

void GameBoard2::layout(ut::rect const& bounds)
{
    gb.layout(bounds);

    usr.layout(gb.usr.player,
               gb.usr.melee,
               gb.usr.ranged,
               gb.usr.siege,
               gb.usr.stats);
    usr.deck.setTestCards(50);

    cpu.layout(gb.cpu.player,
               gb.cpu.melee,
               gb.cpu.ranged,
               gb.cpu.siege,
               gb.cpu.stats);
    cpu.deck.setTestCards(50);



}

void GameBoard2::update(float dt)
{
    usr.update(dt);
    cpu.update(dt);

    auto mp = tout(GetMousePosition());

    if (CardRef ref; usr.tryGetHoveredCard(mp, ref))
    {
        if (ref != card_hover)
        {
            card_hover = ref;
            TANK.elevateCard(card_hover.id);
        }
    }
    else
    {
        card_hover.reset();
    }
}

void GameBoard2::drawAboveCards()
{
    usr.drawAboveCards();
    cpu.drawAboveCards();

    if (card_hover)
    {
        auto p = card_hover->layout.getRect().center();
        auto radius = card_hover->layout.m_w/3;

        drawCircle(p, radius, colors::greenyellow.withNormalA(.5));
    }

}

void GameBoard2::drawUnderCards()
{
    usr.drawUnderCards();
    cpu.drawUnderCards();
}

void GameBoard2::drawDebug()
{
    if (ImGui::Button("Draw"))
    {
        auto idx = 0;
        boss.stackToRow(usr.deck, usr.hand, idx);
    }

    ImGui::SameLine();

    if (ImGui::Button("Discard"))
    {
        auto idx = usr.hand.numCards()-1;
        boss.rowToStack(usr.hand, usr.yard, idx);
    }

    ImGui::SameLine();

    auto& a = usr.siege.units;
    auto& b = usr.hand;

    if (a.isEmpty() && b.isEmpty())
    {
        ImGui::PushItemDisabled();
        ImGui::Button("Swap");
        ImGui::PopItemDisabled();
    }
    else if (ImGui::Button("RowToRow"))
    {
        if (a.numCards() < b.numCards())
        {
            boss.rowToRow(b, a,
                          RNG.nextu(b.numCards()),
                          RNG.nextu(a.numCards()+1));
        }
        else
        {

            boss.rowToRow(a, b,
                          RNG.nextu(a.numCards()),
                          RNG.nextu(b.numCards()+1));
        }
    }

    auto& c = usr.siege.special;

    if (b.isEmpty())
    {
        ImGui::PushItemDisabled();
        ImGui::Button("RowToSpot");
        ImGui::PopItemDisabled();
    }
    else if (ImGui::Button("RowToSpot"))
    {
        if (c.isEmpty())
        {
            boss.rowToSlot(b, c, RNG.nextu(b.numCards()));
        }
        else
        {
            boss.slotToRow(c, b, RNG.nextu(b.numCards()+1));
        }
    }

    gb.drawDebug();
    usr.drawDebug();
    cpu.drawDebug();

}