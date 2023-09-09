//
// Created by james on 9/7/23.
//

#pragma once

#include "game/card.hpp"

namespace game
{
    class CardContainer
    {
    public:
        virtual void        setGhost(size_t idx)=0;
        virtual void        clearGhost()=0;
        virtual bool        hasGhost() const=0;
        virtual ut::rect    getGhostRect() const=0;
        virtual bool        tryGetGhostIndex(ut::vec2 const& mp, size_t& idx)=0;

        virtual void        setHover(size_t idx)=0;
        virtual void        clearHover()=0;
        virtual bool        hasHover() const=0;
        virtual ut::rect    getHoverRect() const=0;
        virtual bool        tryGetHoveredIndex(ut::vec2 const& mp, size_t& idx)=0;

        //virtual size_t      count()=0;
        //virtual ut::rect    addCard(size_t idx, Card const& card)=0;
        //virtual Card        removeCard(size_t idx)=0;

        virtual void dropCard(size_t idx, Card const& card)=0;
        virtual void throwCard(size_t idx, Card const& card)=0;
        virtual Card removeCard(size_t idx)=0;


    private:

    };
}