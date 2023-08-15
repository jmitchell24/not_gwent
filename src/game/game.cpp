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
}

void Card::draw()
{
    assert(m_is_layout_ready);

    VIRT_DEBUG(rect());

    auto r = rect();
    auto c = tint();

    VIRT.drawTexture(m_texture, r, c);


#ifndef NDEBUG

    auto outer = m_outline_color.toHSLUV();
    auto inner = outer.withL(outer.l / 2).withA(0.75f);

    VIRT.drawRectangle(r, inner.toColor());
    VIRT.drawRectangleLines(r, 5.0f, outer.toColor());
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

    Card card{texture_decoy};

#ifndef NDEBUG

    card.m_outline_color = RANDOM.nextColor();
#endif

    return card;
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

    m_calc_ghosted = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size() + 1);
    m_calc_hovered = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size());

    m_idx_hovered   = -1;
    m_idx_ghosted   = -1;

#ifndef NDEBUG
    m_is_layout_ready = true;
#endif

    for (size_t i = 0; i < m_cards.size(); ++i)
    {
        auto&& it = m_cards[i];
        it.layout({m_card_width, m_card_height});
        it.setPosition(m_calc_hovered.getPos(i));
    }
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
    VIRT_DEBUG(CardList::m_calc_ghosted.bounds);
    VIRT_DEBUG(CardList::m_calc_hovered.bounds);

    VIRT.drawRectangleLines(m_bounds, 2.0f, colors::white);
    VIRT.drawRectangle(m_bounds, colors::darkslategrey);

    for (size_t i = 0; i < m_cards.size(); ++i)
    {
        if (m_idx_hovered == i)
            continue;
        m_cards[i].draw();
    }

    if (m_idx_hovered >= 0)
        m_cards[m_idx_hovered].draw();

    if (m_idx_ghosted >= 0)
    {
        auto r = m_calc_ghosted.getRect(m_idx_ghosted);
        VIRT.drawRectangle(r, colors::greenyellow.withNormalA(0.5f));
    }
}

void CardList::setGhost(size_t idx)
{
    assert(idx <= m_cards.size());
    if (m_idx_ghosted != idx)
    {
        m_idx_ghosted = (ssize_t)idx;

        for (size_t i = 0; i < m_idx_ghosted; ++i)
        {
            m_cards[i].targetPosition(m_calc_ghosted.getPos(i));
        }

        for (size_t i = m_idx_ghosted; i < m_cards.size(); ++i)
        {
            m_cards[i].targetPosition(m_calc_ghosted.getPos(i+1));
        }
    }
}

void CardList::clearGhost()
{
    if (m_idx_ghosted >= 0)
    {
        for (size_t i = 0; i < m_cards.size(); ++i)
        {
            m_cards[i].targetPosition(m_calc_hovered.getPos(i));
        }
        m_idx_ghosted = -1;
    }
}

void CardList::setHover(size_t idx)
{
    assert(idx < m_cards.size());
    hover((ssize_t)idx);
}

void CardList::clearHover()
{
    hover(-1);
}

bool CardList::tryGetHoverIndex(vec2 const& mp, size_t& idx) const
{
    assert(m_is_layout_ready);
    return m_calc_hovered.tryGetIndex(mp, idx);
}

bool CardList::tryGetGhostIndex(vec2 const& mp, size_t& idx) const
{
    assert(m_is_layout_ready);
    return m_calc_ghosted.tryGetIndex(mp, idx);
}

void CardList::addCard(size_t idx, Card const& card, AddAnim anim)
{
    assert(m_is_layout_ready);
    assert(idx <= m_cards.size());

    clearHover();
    clearGhost();

    Card& it = *m_cards.insert(m_cards.begin() + idx, card);
    it.layout({m_card_width, m_card_height});

    m_calc_ghosted = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size() + 1);
    m_calc_hovered = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size());

    switch (anim)
    {
        case ANIM_NONE:
            it.setElevation(0.0f);
            it.setOpacity(1.0f);
            //it.setPosition(calcCardPos(idx, m_cards.size()));
            it.setPosition(m_calc_hovered.getPos(idx));
            break;

        case ANIM_MOVE:
            it.setElevation(0.0f);
            it.setOpacity(1.0f);
            //it.targetPosition(calcCardPos(idx, m_cards.size()));
            //it.targetPosition(m_calc_hovered.getPos(idx));
            break;

        case ANIM_CREATE:
            it.setElevation(3.0f);
            it.targetElevation(0.0f);
            it.setOpacity(0.5f);
            it.targetOpacity(1.0f);

            //it.setPosition(calcCardPos(idx, m_cards.size()));
            it.setPosition(m_calc_hovered.getPos(idx));
            break;

        default:
            assert_case(AddAnim);
            break;
    }

    moveAllCards();
}

Card CardList::removeCard(size_t idx)
{
    assert(m_is_layout_ready);
    assert(idx < m_cards.size());

    clearHover();
    clearGhost();

    Card card = m_cards[idx];

    m_cards.erase(m_cards.begin()+idx);
    m_calc_ghosted = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size() + 1);
    m_calc_hovered = CardCalc::create(m_direction, m_card_width, m_bounds_pad, m_cards.size());

    moveAllCards();

    return card;
}







//vec2 CardList::calcCardPos(size_t idx, size_t num_cards) const
//{
//    assert(m_is_layout_ready);
//    assert(idx < num_cards);
//
//    float cards_width  = m_card_width * float(num_cards);
//    float bounds_width = m_bounds_pad.width();
//
//
//    if (cards_width > bounds_width)
//    {
//        auto card_origin    = m_bounds_pad.withSize(m_card_width, m_card_height);
//        auto card_increment = (bounds_width - m_card_width) / float(num_cards-1);
//        return card_origin.pos().withOffsetX(card_increment * float(idx));
//    }
//
//    switch (m_direction)
//    {
//        case DIR_LEFT:
//        {
//            auto card_origin    = m_bounds_pad.withSize(m_card_width, m_card_height);
//            auto card_increment = m_card_width;
//            return card_origin.pos().withOffsetX(card_increment * float(idx));
//        }
//
//        case DIR_CENTER:
//        {
//            auto card_origin    = m_bounds_pad.withOffsetX( (bounds_width - cards_width) / 2 ).withSize(m_card_width, m_card_height);
//            auto card_increment = m_card_width;
//            return card_origin.pos().withOffsetX(card_increment * float(idx));
//        }
//
//        default: assert_case(Direction); break;
//    }
//
//    return {0,0};
//}

//
// CardList::CardCalc -> Implementation
//

CardList::CardCalc CardList::CardCalc::create(Direction direction, float card_width, rect const& bounds, size_t card_count)
{
    if (float w = card_width * float(card_count); w < bounds.width())
    {
        switch (direction)
        {
            case DIR_LEFT  : return { bounds.anchorTLtoTL(w, bounds.height()), card_count, card_width, card_width };
            case DIR_CENTER: return { bounds.anchorCCtoCC(w, bounds.height()), card_count, card_width, card_width };

            default: assert_case(Direction);
        }

        return { {}, 0, 0.0f, 0.0f };
    }

    return { bounds, card_count, card_width, (bounds.width() - card_width) / float(card_count-1) };
}

bool CardList::CardCalc::tryGetIndex(ut::vec2 const& mp, size_t& idx) const
{
    if (bounds.contains(mp))
    {
        auto mp_min   = bounds.min.x;
        auto mp_max   = bounds.max.x;
        auto mp_width = mp_max - mp_min;

        idx = size_t( ((mp.x - mp_min) / (mp_width)) * float(card_count) );
        return true;
    }
    return false;
}

vec2 CardList::CardCalc::getPos(size_t idx) const
{
    return bounds.pos().withOffsetX(card_gap * float(idx));
}

rect CardList::CardCalc::getRect(size_t idx) const
{
    return bounds.withWidth(card_width).withOffsetX(card_gap * float(idx));
}

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











































