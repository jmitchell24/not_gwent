//
// Created by james on 9/15/23.
//

#include <ut/random.hpp>
#include "game/game2.hpp"
using namespace game;

#include "rlImGui/imgui/imgui_extra.hpp"

#include "gfx/gfx_virt2d.hpp"

//
// CardRow
//

//void CardRow::rebuildLayout()
//{
//    auto l = Card::Layout::fromHeight(bounds.height());
//    layout_hover = CardLayout::create(CardLayout::DIR_CENTER, bounds, l.w, ids.size());
//    layout_hover = CardLayout::create(CardLayout::DIR_CENTER, bounds, l.w, ids.size()+1);
//}
//
//void CardRow::layout(ut::rect const& b)
//{
//    bounds = b;
//    rebuildLayout();
//}
//
//bool CardRow::containsID(CardID id)
//{
//    for (auto&& it : ids)
//        if (id == it)
//            return true;
//    return false;
//}
//
//void CardRow::addID(size_t idx, CardID id)
//{
//    assert(!containsID(id));
//    assert(idx <= ids.size());
//
//    ids.insert(ids.begin()+ssize_t(idx), id);
//    rebuildLayout();
//}
//
//void CardRow::removeID(CardID id)
//{
//    assert(containsID(id));
//
//    for (auto it = ids.begin(); it != ids.end(); ++it)
//    {
//        if (id == *it)
//        {
//            ids.erase(it);
//            rebuildLayout();
//            return;
//        }
//    }
//}

//
// GameBoard2
//

void GameBoard2::layout(ut::rect const& bounds)
{
    gb.layout(bounds);

    row1.layout(gb.combat_usr_melee.units);
    row2.layout(gb.combat_cpu_melee.units);
    spinner.layout(gb.combat_usr_melee.score);
}

void GameBoard2::update(float dt)
{
    tank.update(dt);
    spinner.update(dt);
}

void GameBoard2::draw()
{
    tank.draw();
    spinner.draw();
}

void GameBoard2::drawDebug()
{
    ut::rangen rg;
    if (ImGui::Button("+"))
    {
        auto idx = rg.nexti(row1.ids.size());
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
        auto idx = rg.nexti(row1.ids.size()-1);

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