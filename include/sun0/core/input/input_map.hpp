/*************************************************************************/
/*  input_map.hpp                                                        */
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
#pragma once

#include <vector>
#include <unordered_map>

namespace sun {

class Event;

class InputMap
{
public:

    struct Action {
        bool pressed    = false;
        float strength  = 0.f;
    };

    void update_actions(const Event&);



    static InputMap& instance();

private:

    InputMap();

    InputMap(const InputMap&) = delete;

    InputMap(InputMap&&) = delete;

    std::unordered_map<Event, std::string>  action_lookup_;
    std::unordered_map<std::string, Action> action_map_;
};

}
