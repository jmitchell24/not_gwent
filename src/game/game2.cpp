//
// Created by james on 9/15/23.
//


#include "game/game2.hpp"
#include "game/asset/assets.hpp"
using namespace game;

//
// ng
//
#include "ng/ng_card_data.hpp"

//
// gfx
//
#include "gfx/gfx_draw.hpp"
#include "rlImGui/imgui/imgui_mods.hpp"
using namespace gfx;

//
// ledit
//
#include "ledit/ledit_editor.hpp"
using namespace ledit;

//
// ut
//
#include <ut/check.hpp>
#include <ut/random.hpp>
#include <ut/random.hpp>
using namespace ut;

//
// std
//

#include <algorithm>
using namespace std;

//
//
//

static BoxEditor BOX_EDITOR_GAMEBOARD {"GameBoard"};
static BoxEditor BOX_EDITOR_CARDPICKER { "CardPicker" };

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
        ng::cards::neutral_dummy_1,
        ng::cards::neutral_dummy_2,
        ng::cards::neutral_dummy_3,
        ng::cards::special_frost_1,
        ng::cards::special_frost_2,
        ng::cards::special_frost_3,
        ng::cards::special_clearsky_1,
        ng::cards::special_clearsky_2,
        ng::cards::special_clearsky_3,
        ng::cards::special_horn_1,
        ng::cards::special_horn_2,
        ng::cards::special_horn_3,
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

void GameBoard2::layout(rect const& bounds)
{
    gb.layout(bounds);
// *** auto-generated by ledit ***
    gb.cpu.player.deck             = rect( 345.500f,   10.000f,  413.780f,   88.750f);
    gb.cpu.player.hand             = rect( 423.780f,   10.000f,  856.220f,   88.750f);
    gb.cpu.player.yard             = rect( 866.220f,   10.000f,  934.500f,   88.750f);
    gb.cpu.stats.avatar            = rect(  34.771f,  122.735f,  186.075f,  274.040f);
    gb.cpu.stats.deck_name         = rect(  20.000f,   50.912f,  200.846f,   81.823f);
    gb.cpu.stats.gems              = rect( 210.846f,   20.000f,  325.500f,  142.020f);
    gb.cpu.stats.lead_name         = rect(  20.000f,   81.823f,  200.846f,  112.735f);
    gb.cpu.stats.name              = rect(  20.000f,   20.000f,  200.846f,   50.912f);
    gb.cpu.stats.score             = rect( 210.846f,  152.020f,  325.500f,  274.040f);
    gb.usr.player.deck             = rect( 345.500f,  631.250f,  413.780f,  710.000f);
    gb.usr.player.hand             = rect( 423.780f,  631.250f,  856.220f,  710.000f);
    gb.usr.player.yard             = rect( 866.220f,  631.250f,  934.500f,  710.000f);
    gb.usr.stats.avatar            = rect(  34.771f,  548.695f,  186.075f,  700.000f);
    gb.usr.stats.deck_name         = rect(  20.000f,  476.872f,  200.846f,  507.783f);
    gb.usr.stats.gems              = rect( 210.846f,  445.960f,  325.500f,  567.980f);
    gb.usr.stats.lead_name         = rect(  20.000f,  507.783f,  200.846f,  538.695f);
    gb.usr.stats.name              = rect(  20.000f,  445.960f,  200.846f,  476.872f);
    gb.usr.stats.score             = rect( 210.846f,  577.980f,  325.500f,  700.000f);
// *** auto-generated by ledit ***





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

    card_picker.layout();



    usr.deck.setCards(getNGTestDeck());
    usr.lead.setCard(getNGTestLeader());

    cpu.deck.setCards(getNGTestDeck());
    cpu.lead.setCard(getNGTestLeader());

    for (int i = 0; i < 10; ++i)
    {
        boss.stackToRow(usr.deck, usr.hand, i);
        boss.stackToRow(cpu.deck, cpu.hand, i);
    }

    BOX_EDITOR_GAMEBOARD.setRoot(bounds);
    BOX_EDITOR_GAMEBOARD.setFile("data/layout/gb.yaml");

    BOX_EDITOR_CARDPICKER.setRoot(bounds);
    BOX_EDITOR_CARDPICKER.setFile("data/layout/cp.yaml");
}

bool GameBoard2::tryGetHoveredCard(vec2 const& mp, CardRef& ref)
{
    if (card_picker.isOpen())
        return card_picker.tryGetHoveredCard(mp, ref);

    return
        usr.tryGetHoveredCard(mp, ref) ||
        cpu.tryGetHoveredCard(mp, ref);
}

void GameBoard2::update(update_param u)
{
    usr.update(u);
    cpu.update(u);
    boss.update(u);
    card_picker.update(u);

    auto mp = u.view_mouse_pos;

    BOX_EDITOR_GAMEBOARD.view_transform = u.view_transform;
    BOX_EDITOR_CARDPICKER.view_transform = u.view_transform;

    if (BOX_EDITOR_GAMEBOARD.wantCaptureMouse())
        return;
    if (BOX_EDITOR_CARDPICKER.wantCaptureMouse())
        return;

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

    auto usr_params = CastTargetParams{ usr, cpu, card_picker, boss };
    auto cpu_params = CastTargetParams{ cpu, usr, card_picker, boss };

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (Cast cast; usr.tryCast(usr_params, cast, mp))
        {
            std::visit(visitors::CardCaster{*this, usr, cpu}, cast);
            updateScores(usr);
            updateScores(cpu);
        }

        if (Cast cast; cpu.tryCast(cpu_params, cast, mp))
        {
            std::visit(visitors::CardCaster{*this, cpu, usr}, cast);
            updateScores(usr);
            updateScores(cpu);
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        usr.cancelCast( usr_params );
        cpu.cancelCast( cpu_params );
    }


}

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

void GameBoard2::drawUnderBoardCards()
{
    usr.drawUnderCards();
    cpu.drawUnderCards();
}

void GameBoard2::drawAboveBoardCards()
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

void GameBoard2::drawUnderOverlayCards()
{
    card_picker.drawUnderCards();
}

void GameBoard2::drawAboveOverlayCards()
{
    card_picker.drawAboveCards();
}

void GameBoard2::drawDebug()
{
    using namespace ImGui;

    if (BOX_EDITOR_GAMEBOARD.draw())
    {
#define TRY_GET_RECT(x_) BOX_EDITOR_GAMEBOARD.tryGetBorder(#x_, x_)

        {
            if (TRY_GET_RECT(gb.usr.player.hand))
                usr.hand.layout(gb.usr.player.hand);

            if (TRY_GET_RECT(gb.usr.player.deck))
                usr.deck.layout(gb.usr.player.deck);

            if (TRY_GET_RECT(gb.usr.player.yard))
                usr.yard.layout(gb.usr.player.yard);

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.usr.stats.name     );
                b|=TRY_GET_RECT(gb.usr.stats.deck_name);
                b|=TRY_GET_RECT(gb.usr.stats.lead_name);
                b|=TRY_GET_RECT(gb.usr.stats.gems     );
                b|=TRY_GET_RECT(gb.usr.stats.score    );
                b|=TRY_GET_RECT(gb.usr.stats.avatar   );
                if (b) usr.stats.layout(gb.usr.stats);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.usr.melee.units);
                b|=TRY_GET_RECT(gb.usr.melee.buff);
                b|=TRY_GET_RECT(gb.usr.melee.nerf);
                b|=TRY_GET_RECT(gb.usr.melee.score);
                if (b) usr.melee.layout(gb.usr.melee);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.usr.ranged.units);
                b|=TRY_GET_RECT(gb.usr.ranged.buff);
                b|=TRY_GET_RECT(gb.usr.ranged.nerf);
                b|=TRY_GET_RECT(gb.usr.ranged.score);
                if (b) usr.ranged.layout(gb.usr.ranged);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.usr.siege.units);
                b|=TRY_GET_RECT(gb.usr.siege.buff);
                b|=TRY_GET_RECT(gb.usr.siege.nerf);
                b|=TRY_GET_RECT(gb.usr.siege.score);
                if (b) usr.siege.layout(gb.usr.siege);
            }
        }

        {
            if (TRY_GET_RECT(gb.cpu.player.hand))
                cpu.hand.layout(gb.cpu.player.hand);

            if (TRY_GET_RECT(gb.cpu.player.deck))
                cpu.deck.layout(gb.cpu.player.deck);

            if (TRY_GET_RECT(gb.cpu.player.yard))
                cpu.yard.layout(gb.cpu.player.yard);

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.cpu.stats.name     );
                b|=TRY_GET_RECT(gb.cpu.stats.deck_name);
                b|=TRY_GET_RECT(gb.cpu.stats.lead_name);
                b|=TRY_GET_RECT(gb.cpu.stats.gems     );
                b|=TRY_GET_RECT(gb.cpu.stats.score    );
                b|=TRY_GET_RECT(gb.cpu.stats.avatar   );
                if (b) cpu.stats.layout(gb.cpu.stats);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.cpu.melee.units);
                b|=TRY_GET_RECT(gb.cpu.melee.buff);
                b|=TRY_GET_RECT(gb.cpu.melee.nerf);
                b|=TRY_GET_RECT(gb.cpu.melee.score);
                if (b) cpu.melee.layout(gb.cpu.melee);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.cpu.ranged.units);
                b|=TRY_GET_RECT(gb.cpu.ranged.buff);
                b|=TRY_GET_RECT(gb.cpu.ranged.nerf);
                b|=TRY_GET_RECT(gb.cpu.ranged.score);
                if (b) cpu.ranged.layout(gb.cpu.ranged);
            }

            {
                bool b = false;
                b|=TRY_GET_RECT(gb.cpu.siege.units);
                b|=TRY_GET_RECT(gb.cpu.siege.buff);
                b|=TRY_GET_RECT(gb.cpu.siege.nerf);
                b|=TRY_GET_RECT(gb.cpu.siege.score);
                if (b) cpu.siege.layout(gb.cpu.siege);
            }
        }




#undef TRY_GET_RECT
    }

    if (BOX_EDITOR_CARDPICKER.draw())
    {
        bool b = false;
#define TRY_GET_RECT(x_) BOX_EDITOR_CARDPICKER.tryGetBox(#x_, x_)
        b |= TRY_GET_RECT(card_picker.m_layout.card_choice);
        b |= TRY_GET_RECT(card_picker.m_layout.card_options);
        b |= TRY_GET_RECT(card_picker.m_layout.title);
        b |= TRY_GET_RECT(card_picker.m_layout.background);
        b |= TRY_GET_RECT(card_picker.m_layout.confirm);
        b |= TRY_GET_RECT(card_picker.m_layout.cancel);
#undef TRY_GET_RECT

        if (b) card_picker.layout();
    }

    if (Button("Draw"))
        OpenPopup("draw_popup");

    if (BeginPopup("draw_popup"))
    {
        if (MenuItem("Top Deck"))
        {
            boss.stackToRow(usr.deck, usr.hand, 0);
        }

#define TOP_DECK(x_) if (MenuItem(PRINTER("%s", (x_).name.c_str()))) { usr.deck.pushNgCard((x_)); boss.stackToRow(usr.deck, usr.hand, 0); }
        if (BeginMenu("Weather"))
        {
            TOP_DECK(ng::cards::special_frost_1)
            TOP_DECK(ng::cards::special_clearsky_1)
            TOP_DECK(ng::cards::special_fog_1)
            TOP_DECK(ng::cards::special_horn_1)
            TOP_DECK(ng::cards::special_rain_1)
            TOP_DECK(ng::cards::special_skellige_storm_1)
            EndMenu();
        }

        TOP_DECK(ng::cards::special_scorch_1)
        TOP_DECK(ng::cards::neutral_dummy_1)
        TOP_DECK(ng::cards::neutral_yennefer)
#undef TOP_DECK

        EndPopup();
    }


    if (Button("Discard"))
    {
        auto idx = usr.hand.numCards()-1;
        boss.rowToStack(usr.hand, usr.yard, idx);
    }

    gb.drawDebug();
    usr.drawDebug();
    cpu.drawDebug();


    if (card_hover)
    {
        auto& ng = card_hover->ng;

        BeginTooltip();




        BeginStruct(ng.name);

        StructField("id"         , PRINTER("%d", ng.id));
        StructField("type"       , ng::toString(ng.type));
        StructField("deck"       , ng::toString(ng.deck));
        StructField("expansion"  , ng::toString(ng.expansion));
        StructField("filename"   , ng.filename);

        EndStruct();

        struct V
        {
            void operator()(ng::NilCard const& c) { }

            void operator()(ng::SpecialCard const& c)
            {
                BeginStruct("Special");
                StructField("type", ng::toString(c.type));
                EndStruct();
            }

            void operator()(ng::UnitCard const& c)
            {
                BeginStruct("Unit");
                StructField("strength"   , PRINTER("%d", c.strength));
                StructField("is_hero"    , c.is_hero ? "yes" : "no");
                StructField("ability"    , ng::toString(c.ability));
                StructField("row"        , ng::toString(c.row));
                EndStruct();
            }

            void operator()(ng::LeaderCard const& c)
            {
                BeginStruct("Leader");
                StructField("type", ng::toString(c.type));
                EndStruct();
            }
        };

        ng.visit(V{});




        ImGui::EndTooltip();
    }
}

void GameBoard2::drawDebugMenu()
{
    BOX_EDITOR_GAMEBOARD.drawMenu();
    BOX_EDITOR_CARDPICKER.drawMenu();
}


#if 0
void GameBoard2::playCard(Player& player, size_t idx)
{
    CardRef ref = player.hand.getCard(idx);
    ng::Card ng = ref->ng;

    switch (ng.type)
    {
        case ng::Card::TYPE_SPECIAL:
        {
            auto special = ng.asSpecialCard();
            break;
        }

        case ng::Card::TYPE_UNIT:
        {
            auto unit = ng.asUnitCard();

            break;
        }

        default:
            nopath_case(ng::Card::Type);
            break;
    }
    if (ng.isUnitCard())
    {
        auto unit = ref->ng.asUnitCard();
        switch (unit.row)
        {

            case ng::UnitCard::ROW_MELEE    : boss.rowToRow(player.hand, player.melee.units , idx, 0); updateScores(player); break;
            case ng::UnitCard::ROW_RANGED   : boss.rowToRow(player.hand, player.ranged.units, idx, 0); updateScores(player); break;
            case ng::UnitCard::ROW_SIEGE    : boss.rowToRow(player.hand, player.siege.units, idx, 0); updateScores(player); break;

            //default:assert_case(ng::UnitCard::Row);
        }


    }
    else if (ng.isSpecialCard())
    {
        if (ref->ng.asSpecialCard().type == ng::SpecialCard::SPECIAL_CMDR_HORN)
        {
            //boss.rowToSlot(player.hand, )
        }
    }
}
#endif