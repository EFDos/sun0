/*************************************************************************/
/*  input_map.cpp                                                        */
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
#include "input_map.hpp"
#include "core/event.hpp"

namespace sun {

InputMap::InputMap()
{}

InputMap& InputMap::instance()
{
    static InputMap singleton;
    return singleton;
}

void InputMap::update_actions(Event& event)
{
    auto it = action_lookup_.find(event);

    if (it == action_lookup_.end()) {
        return;
    }

    if (event.type == EventType::KeyPressed ||
        event.type == EventType::MouseButtonPressed
        event.type == EventType::JoystickButtonPressed)
    {
        it->second.pressed = true;
        return;
    }

    if (event.type == EventType::KeyReleased ||
        event.type == EventType::MouseButtonReleased ||
        event.type == EventType::JoystickButtonReleased)
    {
        it->second.pressed = false;
        return;
    }

    if (event.type == EventType::JoystickAxisMoved)
    {
        it->second.strength = event
    }
}

}
