//
// Created by james on 9/29/23.
//

#include "game/game_stats.hpp"
#include "game/game_combat_row.hpp"
#include "game/game_target.hpp"
#include "game/game_cast.hpp"

namespace game
{

    class Player
    {
    public:
        board::BoardStack  deck{board::BoardStack::DECK};
        board::BoardStack  yard{board::BoardStack::YARD};
        board::BoardSlot   lead;
        board::BoardRow    hand;

        CombatRow melee;
        CombatRow ranged;
        CombatRow siege;

        Stats stats;



        void layout(
                layout::PlayerRow const& row_player,
                layout::CombatRow const& row_melee,
                layout::CombatRow const& row_ranged,
                layout::CombatRow const& row_siege,
                layout::StatsBoard const& player_stats);

        bool tryGetHoveredCard(ut::vec2 const& mp, CardRef& ref);

        Target target = TargetDefault{};

        void clearAllHighlights()
        {
            melee.clearHighlight();
            ranged.clearHighlight();
            siege.clearHighlight();
        }

        void changeTarget(Target t);
        bool tryCast(ut::vec2 const& mp, Cast& cast);
        void cancelCast();

        void update(float dt);
        void drawAboveCards();
        void drawUnderCards();
        void drawDebug();
    };


    struct ChangePlayerTarget
    {
        Player& player;
        void operator()(TargetDefault const& t);
        void operator()(TargetUnitRow const& t);
        void operator()(TargetUnitCard const& t);
        void operator()(TargetCommanderHorn const& t);
    };

    struct CastTarget
    {
        Player&             player;
        ut::vec2 const&     mp;
        Cast&               cast;

        bool operator()(TargetDefault const& t);
        bool operator()(TargetUnitRow const& t) ;
        bool operator()(TargetUnitCard const& t) ;
        bool operator()(TargetCommanderHorn const& t) ;
    };
}