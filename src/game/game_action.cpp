//
// Created by james on 3/4/24.
//

#include "game/game_action.hpp"
using namespace game;
using namespace game::visitors;

#if 0 // stub
void ExecuteAction::operator() (ActionNop const&)
{
    TraceLog(LOG_INFO, "ACTION NOP");
}
#endif

void ExecuteAction::operator() (ActionNop const&)
{
    TraceLog(LOG_INFO, "ACTION NOP");
}

void ExecuteAction::operator() (ActionMoveCard const& a)
{
//    auto& b = g.boss;
//    auto& p = g.usr;
//
//    switch (a.row)
//    {
//
//        case RowType::ROW_MELEE : b.rowToRow(p.hand, p.melee.units , a.hand_idx, 0); break;
//        case RowType::ROW_RANGED: b.rowToRow(p.hand, p.ranged.units, a.hand_idx, 0); break;
//        case RowType::ROW_SIEGE : b.rowToRow(p.hand, p.siege.units , a.hand_idx, 0); break;
//        default:nopath_case(CastUnit::Row);
//    }

    TraceLog(LOG_INFO, "ACTION MOVE HAND TO ROW");
}

void ExecuteAction::operator() (ActionScorch const&)
{
    TraceLog(LOG_INFO, "ACTION SCORCH");
}

void ExecuteAction::operator() (ActionUnitListBuff const&)
{
    TraceLog(LOG_INFO, "ACTION UNIT LIST BUFF");
}

void ExecuteAction::operator() (ActionUnitListNerf const&)
{
    TraceLog(LOG_INFO, "ACTION UNIT LIST NERF");
}