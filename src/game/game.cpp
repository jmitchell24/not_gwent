#include "game/game.hpp"

#include <utility>
using namespace game;

#include "random.hpp"
#include "assert.hpp"

//
// gfx
//
#include "gfx/gfx_virt2d.hpp"
#include "game/types.hpp"

using namespace gfx;

//
// ut
//
using namespace ut;

//
// std
//
using namespace std;

size_t constexpr static VIRT_WIDTH  = 720;
size_t constexpr static VIRT_HEIGHT = 1280;
size_t constexpr static VIRT_PAD    = 10;

//
// Card -> Implementation
//

Card::Card()
    : Card(createTestCard())
{}

Card::Card(Texture2D const& texture) :
    m_texture {texture}
{
    assert(IsTextureReady(texture));
}

void Card::setPosition(vec2 const& p)
{
    assert(m_is_layout_ready);

    m_psize.pos = p;
    m_tween_pos.setToDst(p);
}

void Card::setElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_elevation = f;
    m_tween_elevation.setToDst(f);
}

void Card::setOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);
    assert(f <= 1.0f);

    m_opacity = f;
    m_tween_opacity.setToDst(f);
}


void Card::targetPosition(vec2 const& p)
{
    assert(m_is_layout_ready);

    m_tween_pos.animTo(p);
}

void Card::targetElevation(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);

    m_tween_elevation.animTo(f);
}

void Card::targetOpacity(float f)
{
    assert(m_is_layout_ready);
    assert(f >= 0.0f);
    assert(f <= 1.0f);

    m_tween_opacity.animTo(f);
}

void Card::layout(vec2 const& size)
{
    m_psize.size = size;

    m_elevation = 0.0f;
    m_opacity   = 1.0f;

    m_tween_pos         = Tween2::make(&easings::elasticOut , 1.0f, m_psize.pos);
    m_tween_elevation   = Tween1::make(&easings::bounceOut  , 0.5f, m_elevation);
    m_tween_opacity     = Tween1::make(&easings::expoOut    , 0.5f, m_opacity);

#ifndef NDEBUG
    m_is_layout_ready = true;
    m_outline_color = RANDOM.nextColor();
#endif
}

void Card::update()
{
    assert(m_is_layout_ready);

    auto dt = GetFrameTime();
    if (m_tween_pos.update(dt))
    {
        m_psize.pos = m_tween_pos.curVec2();
    }

    if (m_tween_elevation.update(dt))
    {
        m_elevation = m_tween_elevation.curScalar();
    }

    if (m_tween_opacity.update(dt))
    {
        m_opacity = m_tween_opacity.curScalar();
    }

//    // use binary over logical OR to avoid short circuit
//    if (m_tween_pos.update(dt) | m_tween_height.update(dt))
//    {
//        auto s = VIRT_PAD * m_tween_height.curScalar();
//
//        m_bounds_moved = m_bounds_original.withPos(m_tween_pos.curVec2());
//        m_bounds_hovered = m_bounds_moved.expanded( s, s * 1.88f );
//    }
//
//    if (m_tween_opacity.update(dt))
//    {
//        m_tint_color = colors::white.withNormalA(m_tween_opacity.curScalar());
//    }
}

void Card::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(rect());

    auto r = rect();
    auto c = tint();

    VIRT.drawTexture(m_texture, r, c);

#ifndef NDEBUG
    VIRT.drawRectangleLines(r, 5.0f, m_outline_color);
#endif
}

Card Card::createTestCard()
{
    static Texture2D texture_decoy;

    if (!IsTextureReady(texture_decoy))
    {
        texture_decoy = LoadTexture("data/image/decoy.png");
        assert(IsTextureReady(texture_decoy));
    }

    return Card{texture_decoy};
}

cardlist_t Card::createTestCards(size_t n)
{
    cardlist_t cards;
    for (size_t i = 0; i < n; ++i)
        cards.push_back(createTestCard());
    return cards;
}

//
// CardHold -> Implementation
//



//
// CardList -> Implementation
//

CardList::CardList(Direction direction, cardlist_t cards) :
    m_bounds_pad    {},
    m_card_width    {},
    m_card_height   {},
    m_direction     {direction},
    m_cards         {std::move(cards)}
{ }

void CardList::layout(rect const& bounds)
{
    m_bounds        = bounds;
    m_bounds_pad    = bounds.shrunk(VIRT_PAD);

    m_card_height   = m_bounds_pad.height();
    m_card_width    = m_card_height / 1.88f; // aspect ratio of card images from Witcher 3

    m_ptoi_ghosted = PointToIndex::create(m_direction, m_card_width, m_bounds_pad, m_cards.size()+1);
    m_ptoi_hovered = PointToIndex::create(m_direction, m_card_width, m_bounds_pad, m_cards.size());

    m_idx_hovered   = -1;
    m_idx_ghosted   = -1;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif

    size_t num_cards = m_cards.size();
    for (size_t i = 0; i < num_cards; ++i)
    {
        auto&& it = m_cards[i];
        auto p = calcCardPos(i, num_cards);

        it.layout({m_card_width, m_card_height});
        it.setPosition(p);
    }
}

bool CardList::tryGetHoverIndex(vec2 const& mp, ssize_t& idx) const
{
    assert(m_is_layout_ready);

    return m_ptoi_hovered.tryGetIndex(mp, idx);

//    if (!m_cards.empty() && m_bounds_pad.contains(mp))
//    {
//        auto mp_min   = m_bounds_pad.min.x;
//        auto mp_max   = m_bounds_pad.max.x;
//        auto mp_width = mp_max - mp_min;
//
//        if (m_idx_ghosted >= 0)
//        {
//            size_t num_cards    = m_cards.size()+1;
//            float  cards_width  = m_card_width * float(num_cards);
//
//            if (cards_width > mp_width)
//            {
//                if (auto i = ssize_t( ((mp.x - mp_min) / mp_width) * float(num_cards) ); i != m_idx_ghosted)
//                {
//                    if (i < m_idx_ghosted)
//                    {
//                        idx = i;
//                        return true;
//                    }
//
//                    if (i > m_idx_ghosted)
//                    {
//                        idx = i-1;
//                        return true;
//                    }
//                }
//            }
//            else
//                goto LABEL_SIMPLE_BOUNDS_CHECK;
//        }
//        else
//        {
//            size_t num_cards    = m_cards.size();
//            float  cards_width  = m_card_width * float(num_cards);
//
//            if (cards_width > mp_width)
//            {
//                auto i = ssize_t( ((mp.x - mp_min) / (mp_width)) * float(num_cards) );
//                idx = i;
//                return true;
//            }
//            else
//                goto LABEL_SIMPLE_BOUNDS_CHECK;
//        }
//    }
//
//    return false;
//
//    LABEL_SIMPLE_BOUNDS_CHECK:
//    for (size_t i = 0; i < m_cards.size(); ++i)
//    {
//        if (m_cards[i].containsPoint(mp))
//        {
//            idx = (ssize_t)i;
//            return true;
//        }
//    }
//    return false;
}

bool CardList::tryGetGhostIndex (ut::vec2 const& mp, ssize_t& idx) const
{
    assert(m_is_layout_ready);

    return m_ptoi_ghosted.tryGetIndex(mp, idx);
}

size_t CardList::add(size_t idx, Card const& card)
{
    assert(m_is_layout_ready);
    assert(idx <= m_cards.size());

    Card& it = *m_cards.insert(m_cards.begin() + idx, card);
    it.layout({m_card_width, m_card_height});
    it.setPosition(calcCardPos(idx, m_cards.size()));

    moveAllCards();
    return idx;
}

bool CardList::remove(size_t idx)
{
    assert(m_is_layout_ready);

    if (idx < m_cards.size())
    {
        if (m_idx_hovered == idx)
            m_idx_hovered = -1;

        m_cards.erase(m_cards.begin()+idx);

        moveAllCards();
        return true;
    }
    return false;
}

void CardList::update()
{
    assert(m_is_layout_ready);

    for (auto&& it: m_cards)
        it.update();
}

void CardList::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(CardList::m_bounds);
    VIRT_DEBUG(CardList::m_ptoi_ghosted.bounds);
    VIRT_DEBUG(CardList::m_ptoi_hovered.bounds);

    for (size_t i = 0; i < m_cards.size(); ++i)
    {
        if (m_idx_hovered == i)
            continue;
        m_cards[i].draw();
    }

    if (m_idx_hovered >= 0)
        m_cards[m_idx_hovered].draw();

    VIRT.drawRectangleLines(m_bounds, 2.0f, colors::white);

    if (m_idx_ghosted >= 0)
    {
        auto r = calcCardRect(m_idx_ghosted, m_cards.size()+1);
        VIRT.drawRectangle(r, colors::greenyellow.withNormalA(0.5f));
    }
}



ut::vec2 CardList::calcCardPos(size_t idx, size_t num_cards) const
{
    assert(m_is_layout_ready);
    assert(idx < num_cards);

    float cards_width  = m_card_width * float(num_cards);
    float bounds_width = m_bounds_pad.width();


    if (cards_width > bounds_width)
    {
        auto card_origin    = m_bounds_pad.withSize(m_card_width, m_card_height);
        auto card_increment = (bounds_width - m_card_width) / float(num_cards-1);
        return card_origin.pos().withOffsetX(card_increment * float(idx));
    }

    switch (m_direction)
    {
        case DIR_LEFT:
        {
            auto card_origin    = m_bounds_pad.withSize(m_card_width, m_card_height);
            auto card_increment = m_card_width;
            return card_origin.pos().withOffsetX(card_increment * float(idx));
        }

        case DIR_RIGHT:
        {
            auto card_origin    = m_bounds_pad.withOffsetX(-cards_width).withSize(m_card_width, m_card_height);
            auto card_increment = m_card_width;
            return card_origin.pos().withOffsetX(card_increment * float(idx));
        }

        case DIR_CENTER:
        {
            auto card_origin    = m_bounds_pad.withOffsetX( (bounds_width - cards_width) / 2 ).withSize(m_card_width, m_card_height);
            auto card_increment = m_card_width;
            return card_origin.pos().withOffsetX(card_increment * float(idx));
        }

        default: assert_case(Direction); break;
    }

    return {0,0};
}

//void CardList::refreshAllCards(rect const& origin, float increment, CareRefreshType refresh)
//{
//    auto sz = m_cards.size();
//
//    switch (refresh)
//    {
//        case CARD_REFRESH_MOVE:
//            for (size_t i = 0; i < sz; ++i)
//                m_cards[i].targetPosition(origin.pos().withOffsetX(increment * float(i)));
//        break;
//
//        case CARD_REFRESH_LAYOUT:
//            for (size_t i = 0; i < sz; ++i)
//                m_cards[i].layout(origin.withOffsetX(increment * float(i)));
//        break;
//
//        default: assert_case(CareRefreshType); break;
//    }
//}

//void CardList::refresh(CareRefreshType refresh)
//{
//    assert(m_is_layout_ready);
//
//    size_t num_cards    = m_cards.size();
//    float  cards_width  = m_card_width * float(num_cards);
//    float  bounds_width = m_bounds_pad.width();
//
//    if (num_cards > 0)
//    {
//        if (cards_width > bounds_width)
//        {
//            auto card_origin = m_bounds_pad.withSize(m_card_width, m_card_height);
//            auto card_increment = (bounds_width - m_card_width) / float(num_cards-1);
//            refreshAllCards(card_origin, card_increment, refresh);
//        }
//        else
//        {
//            switch (m_direction)
//            {
//                case DIR_LEFT:
//                {
//                    auto card_origin = m_bounds_pad.withSize(m_card_width, m_card_height);
//                    refreshAllCards(card_origin, m_card_width, refresh);
//                    break;
//                }
//
//                case DIR_RIGHT:
//                {
//                    auto card_origin = m_bounds_pad.withOffsetX(-cards_width).withSize(m_card_width, m_card_height);
//                    refreshAllCards(card_origin, m_card_width, refresh);
//                    break;
//                }
//
//                case DIR_CENTER:
//                {
//                    auto card_origin = m_bounds_pad.withOffsetX( (bounds_width - cards_width) / 2 ).withSize(m_card_width, m_card_height);
//                    refreshAllCards(card_origin, m_card_width, refresh);
//                    break;
//                }
//
//                default: assert_case(Direction); break;
//            }
//        }
//    }
//}

//
// StatDisplay -> Implementation
//

StatDisplay::StatDisplay(PlayerType player_type, cstrparam player_name) :
    m_player_type   {player_type},
    m_player_name   {player_name}
{}

void StatDisplay::onLayout(rect const& bounds)
{
    m_bounds_stats = bounds.shrunk(VIRT_PAD).splitNV<5>(VIRT_PAD);
}

void StatDisplay::update()
{
    assert(isLayoutReady());
}

void StatDisplay::draw()
{
    assert(isLayoutReady());

    VIRT_DEBUG(StatDisplay::m_bounds);

    VIRT.drawTextTLtoTL(m_bounds_stats[0], "player:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[0], m_player_name, colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[1], "gems: "_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[1], PRINTER("%d", gems()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[2], "score:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[2], PRINTER("%d", score()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[3], "library:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[3], PRINTER("%d", librarySize()), colors::white);

    VIRT.drawTextTLtoTL(m_bounds_stats[4], "graveyard:"_sv, colors::white);
    VIRT.drawTextTRtoTR(m_bounds_stats[4], PRINTER("%d", graveyardSize()), colors::white);

    VIRT.drawRectThinLines(m_bounds, colors::white);
}

void StatDisplay::load()
{

}

//
// CombatRow -> Implementation
//

CombatRow::CombatRow(CardTarget target)
    : cards { DIR_LEFT, {} }, m_target{target}
{}

CombatRow CombatRow::melee  () { return CombatRow(CARD_TARGET_MELEE); }
CombatRow CombatRow::ranged () { return CombatRow(CARD_TARGET_RANGED); }
CombatRow CombatRow::siege  () { return CombatRow(CARD_TARGET_SIEGE); }


void CombatRow::layout(rect const& bounds)
{
    m_bounds = bounds;
    cards.layout(bounds.shrunk(VIRT_PAD));

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif
}

void CombatRow::update()
{
    cards.update();
}

void CombatRow::draw()
{
    switch (m_target)
    {
        case CARD_TARGET_MELEE : VIRT.drawRectDebug("CombatRow::MELEE::bounds" , m_bounds); break;
        case CARD_TARGET_RANGED: VIRT.drawRectDebug("CombatRow::RANGED::bounds", m_bounds); break;
        case CARD_TARGET_SIEGE : VIRT.drawRectDebug("CombatRow::SIEGE::bounds" , m_bounds); break;
    }

    VIRT.drawRectThinLines(m_bounds, colors::white);

    cards.draw();
}

//
// PlayerBoard -> Implementation
//

void PlayerBoard::onLayout(rect const& bounds)
{
    auto row_bounds = bounds.splitNV<4>(VIRT_PAD);
    auto [bounds_stats, bounds_cards] = row_bounds[0].splitLeft(bounds.width()/3, VIRT_PAD);

    m_cards.layout(bounds_cards);
    m_stats.layout(bounds_stats);

    m_siege.layout(row_bounds[1]);
    m_ranged.layout(row_bounds[2]);
    m_melee.layout(row_bounds[3]);
}


void PlayerBoard::update()
{
    assert(isLayoutReady());

    m_stats.update();
    m_cards.update();
    m_melee.update();
    m_ranged.update();
    m_siege.update();
}

void PlayerBoard::draw()
{
    assert(isLayoutReady());

    VIRT_DEBUG(PlayerBoard::m_bounds);

    m_stats.draw();
    m_cards.draw();
    m_melee.draw();
    m_ranged.draw();
    m_siege.draw();
}











































