//
// Created by james on 9/15/23.
//

#include <ut/check.hpp>
#include <ut/random.hpp>
#include "game/game2.hpp"
using namespace game;

#include "ng/ng_card_data.hpp"

#include "rlImGui/imgui/imgui_mods.hpp"

#include "game/asset/assets.hpp"

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

bool GameBoard2::tryGetHoveredCard(const ut::vec2 &mp, CardRef &ref)
{
    return
        usr.tryGetHoveredCard(mp, ref) ||
        cpu.tryGetHoveredCard(mp, ref);
}

void GameBoard2::update(update_param u)
{
    usr.update(u.frame_time);
    cpu.update(u.frame_time);

    auto mp = u.view_mouse_pos;



    if (CardRef ref; tryGetHoveredCard(mp, ref))
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

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (Cast cast; usr.tryCast(mp, cast))
        {
            std::visit(DoCastingThing{*this}, cast);
            updateScores(usr);
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        usr.cancelCast();
    }
}



//void GameBoard2::playCard(Player& player, size_t idx)
//{
//    CardRef ref = player.hand.getCard(idx);
//    ng::Card ng = ref->ng;
//
//    switch (ng.type)
//    {
//        case ng::Card::TYPE_SPECIAL:
//        {
//            auto special = ng.asSpecialCard();
//            break;
//        }
//
//        case ng::Card::TYPE_UNIT:
//        {
//            auto unit = ng.asUnitCard();
//
//            break;
//        }
//
//        default:
//            nopath_case(ng::Card::Type);
//            break;
//    }
//    if (ng.isUnitCard())
//    {
//        auto unit = ref->ng.asUnitCard();
//        switch (unit.row)
//        {
//
//            case ng::UnitCard::ROW_MELEE    : boss.rowToRow(player.hand, player.melee.units , idx, 0); updateScores(player); break;
//            case ng::UnitCard::ROW_RANGED   : boss.rowToRow(player.hand, player.ranged.units, idx, 0); updateScores(player); break;
//            case ng::UnitCard::ROW_SIEGE    : boss.rowToRow(player.hand, player.siege.units, idx, 0); updateScores(player); break;
//
//            //default:assert_case(ng::UnitCard::Row);
//        }
//
//
//    }
//    else if (ng.isSpecialCard())
//    {
//        if (ref->ng.asSpecialCard().type == ng::SpecialCard::SPECIAL_CMDR_HORN)
//        {
//            //boss.rowToSlot(player.hand, )
//        }
//    }
//}

int GameBoard2::updatePlayerScore(Player& player)
{
    int score =
        player.melee.updateScore() +
        player.ranged.updateScore() +
        player.siege.updateScore();
    player.stats.updateScore(score);
    return score;
}

void GameBoard2::updateScores(Player& player)
{
    updatePlayerScore(player);
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

    gb.drawDebug();
    usr.drawDebug();
    cpu.drawDebug();


    if (card_hover)
    {
        auto& ng = card_hover->ng;

        ImGui::BeginTooltip();




        ImGui::BeginStruct(ng.name);

        ImGui::StructField("id"         , PRINTER("%d", ng.id));
        ImGui::StructField("type"       , ng::toString(ng.type));
        ImGui::StructField("deck"       , ng::toString(ng.deck));
        ImGui::StructField("expansion"  , ng::toString(ng.expansion));
        ImGui::StructField("filename"   , ng.filename);

        ImGui::EndStruct();

        struct V
        {
            void operator()(ng::NilCard const& c) { }

            void operator()(ng::SpecialCard const& c)
            {
                ImGui::BeginStruct("Special");
                ImGui::StructField("type", ng::toString(c.type));
                ImGui::EndStruct();
            }

            void operator()(ng::UnitCard const& c)
            {
                ImGui::BeginStruct("Unit");
                ImGui::StructField("strength"   , PRINTER("%d", c.strength));
                ImGui::StructField("is_hero"    , c.is_hero ? "yes" : "no");
                ImGui::StructField("ability"    , ng::toString(c.ability));
                ImGui::StructField("row"        , ng::toString(c.row));
                ImGui::EndStruct();
            }

            void operator()(ng::LeaderCard const& c)
            {
                ImGui::BeginStruct("Leader");
                ImGui::StructField("type", ng::toString(c.type));
                ImGui::EndStruct();
            }
        };

        ng.visit(V{});




        ImGui::EndTooltip();
    }
}

//
// DoCastingThing
//

void DoCastingThing::operator() (CastUnit const& c)
{
    switch (c.row)
    {

        case CastUnit::MELEE : gb.boss.rowToRow(gb.usr.hand, gb.usr.melee.units , c.hand_idx, 0); break;
        case CastUnit::RANGED: gb.boss.rowToRow(gb.usr.hand, gb.usr.ranged.units, c.hand_idx, 0); break;
        case CastUnit::SIEGE : gb.boss.rowToRow(gb.usr.hand, gb.usr.siege.units , c.hand_idx, 0); break;
        default:nopath_case(CastUnit::Row);
    }
}

void DoCastingThing::operator() (CastSpy const& c)
{
    nopath_impl;
}

void DoCastingThing::operator() (CastCommanderHorn const& c)
{
    switch (c.row)
    {
        case CastCommanderHorn::MELEE : gb.boss.rowToSlot(gb.usr.hand, gb.usr.melee.cmdr_horn , c.hand_idx); break;
        case CastCommanderHorn::RANGED: gb.boss.rowToSlot(gb.usr.hand, gb.usr.ranged.cmdr_horn, c.hand_idx); break;
        case CastCommanderHorn::SIEGE : gb.boss.rowToSlot(gb.usr.hand, gb.usr.siege.cmdr_horn , c.hand_idx); break;

        default:
            nopath_case(CastCommanderHorn::Row);
    }
}

void DoCastingThing::operator() (CastScorch const& c)
{
    nopath_impl;
}
void DoCastingThing::operator() (CastLeaderAbility const& c)
{
    nopath_impl;
}
void DoCastingThing::operator() (CastWeather const& c)
{
    nopath_impl;
}
