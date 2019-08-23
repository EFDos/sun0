/*************************************************************************/
/*  component.cpp                                                        */
/*************************************************************************/
/*                       This file is part of:                           */
/*                            SUN-0 Engine                               */
/*                            sinz.com.br                                */
/*************************************************************************/
/* Copyright (c) 2018-2020 Sinz Group.                                   */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*                                                                       */
/*************************************************************************/
#include "component.hpp"

namespace sun {

Component::Component(Context& context)
:   Object(context),
    id_(0),
    flags_(0x0E),
    owning_entity_(nullptr)
{}

void Component::init()
{
    flags_ |= (uint8)Property::Initialized;
}

void Component::handle_events(Event&)
{}

void Component::update(float delta)
{}

void Component::queue_delete()
{
    flags_ |= (uint8)Property::Terminal;
}

void Component::set_update(bool update)
{
    update ? flags_ |= (uint8)Property::Update :
        flags_ &= ~(uint8)Property::Update;
}

void Component::set_handle_event(bool event)
{
    event ? flags_ |= (uint8)Property::HandleEvent :
        flags_ &= ~(uint8)Property::HandleEvent;
}

void Component::set_draw(bool draw)
{
    draw ? flags_ |= (uint8)Property::Draw :
        flags_ &= ~(uint8)Property::Draw;
}

bool Component::get_update() const
{
    return (flags_ & ((uint8)Property::Update | (uint8)Property::Initialized) &&
            !(flags_ & (uint8)Property::Terminal));
}

bool Component::get_handle_event() const
{
    return (flags_ & ((uint8)Property::HandleEvent | (uint8)Property::Initialized) &&
            !(flags_ & (uint8)Property::Terminal));
}

bool Component::get_draw() const
{
    return (flags_ & (uint8)Property::Draw &&
            flags_ & (uint8)Property::Initialized &&
            !(flags_ & (uint8)Property::Terminal));
}

bool Component::is_terminal() const
{
    return flags_ & (uint8)Property::Terminal;
}

bool Component::is_initialized() const
{
    return flags_ & (uint8)Property::Initialized;
}

}
