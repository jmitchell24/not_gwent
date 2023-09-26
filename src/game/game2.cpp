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

#include <algorithm>
using namespace std;

//
//
//

ng::Card getNGTestLeader()
{
    ng::cardlist_t leaders =
    {
        ng::cards::monsters_eredin_1,
        ng::cards::monsters_eredin_2,
        ng::cards::monsters_eredin_3,
        ng::cards::monsters_eredin_4,
        ng::cards::monsters_eredin_5,
    };

    return leaders[RNG.nextu(leaders.size())];
}

ng::cardlist_t getNGTestDeck()
{
    ng::cardlist_t deck =
    {
        ng::cards::monsters_draug,
        ng::cards::monsters_imlerith,
        ng::cards::monsters_kayran,
        ng::cards::monsters_leshan,
        ng::cards::monsters_arachas_1,
        ng::cards::monsters_arachas_2,
        ng::cards::monsters_arachas_3,
        ng::cards::monsters_arachas_behemoth,
        ng::cards::monsters_botchling,
        ng::cards::monsters_celaeno_harpy,
        ng::cards::monsters_cockatrice,
        ng::cards::monsters_crone_1,
        ng::cards::monsters_crone_2,
        ng::cards::monsters_crone_3,
        ng::cards::monsters_earth_elemental,
        ng::cards::monsters_endrega,
        ng::cards::monsters_fiend,
        ng::cards::monsters_fire_elemental,
        ng::cards::monsters_foglet,
        ng::cards::monsters_frightener,
        ng::cards::monsters_gargoyle,
        ng::cards::monsters_ghoul_1,
        ng::cards::monsters_ghoul_2,
        ng::cards::monsters_ghoul_3,
        ng::cards::monsters_gravehag,
        ng::cards::monsters_gryffin,
        ng::cards::monsters_harpy,
        ng::cards::monsters_frost_giant,
        ng::cards::monsters_toad,
        ng::cards::monsters_nekker_1,
        ng::cards::monsters_nekker_1,
        ng::cards::monsters_nekker_3,
        ng::cards::monsters_plague_maiden,
        ng::cards::monsters_vamp_bruxa,
        ng::cards::monsters_vamp_ekimmara,
        ng::cards::monsters_vamp_fleder,
        ng::cards::monsters_vamp_garkain,
        ng::cards::monsters_vamp_katakan,
        ng::cards::monsters_werewolf,
        ng::cards::monsters_wyvern,
        ng::cards::neutral_ciri_1,
        ng::cards::neutral_dandelion,
        ng::cards::neutral_geralt,
        ng::cards::neutral_avallach,
        ng::cards::neutral_triss,
        ng::cards::neutral_yennefer,
        ng::cards::neutral_emiel,
        ng::cards::neutral_vesemir,
        ng::cards::neutral_villen,
        ng::cards::neutral_zoltan,
        ng::cards::neutral_cow,
        ng::cards::neutral_gaunter,
        ng::cards::neutral_gaunter_darkness_1,
        ng::cards::neutral_gaunter_darkness_2,
        ng::cards::neutral_gaunter_darkness_3,
        ng::cards::neutral_olgierd,
        ng::cards::special_frost_1,
        ng::cards::special_frost_2,
        ng::cards::special_frost_3,
        ng::cards::special_clearsky_1,
        ng::cards::special_clearsky_2,
        ng::cards::special_clearsky_3,
        ng::cards::special_horn_1,
        ng::cards::special_horn_2,
        ng::cards::special_horn_3,
        ng::cards::special_dummy_1,
        ng::cards::special_dummy_2,
        ng::cards::special_dummy_3,
        ng::cards::special_fog_1,
        ng::cards::special_fog_2,
        ng::cards::special_fog_3,
        ng::cards::special_scorch_1,
        ng::cards::special_scorch_2,
        ng::cards::special_scorch_3,
        ng::cards::special_rain_1,
        ng::cards::special_rain_2,
        ng::cards::special_rain_3,
        ng::cards::special_skellige_storm_1,
        ng::cards::special_skellige_storm_2,
        ng::cards::special_skellige_storm_3,
    };

    random_shuffle(deck.begin(), deck.end());

    deck.resize(22);

    return deck;
}

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

    cpu.layout(gb.cpu.player,
               gb.cpu.melee,
               gb.cpu.ranged,
               gb.cpu.siege,
               gb.cpu.stats);

    usr.deck.setCards(getNGTestDeck());
    usr.lead.setCard(getNGTestLeader());

    cpu.deck.setCards(getNGTestDeck());
    cpu.lead.setCard(getNGTestLeader());

    for (int i = 0; i < 10; ++i)
    {
        boss.stackToRow(usr.deck, usr.hand, i);
        boss.stackToRow(cpu.deck, cpu.hand, i);
    }


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