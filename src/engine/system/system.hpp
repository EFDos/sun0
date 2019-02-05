/*************************************************************************/
/*  system.hpp                                                           */
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

#include "common/config.hpp"

#include <string>
#include <unordered_map>

namespace sun {

class SUN_API system
{
public:

    /*enum class type
    {

    };*/

    system();

    virtual ~system();

    virtual void init() = 0;

    virtual void shutdown() = 0;

    virtual const char* get_name() const = 0;

    static void register_instance(system* instance);

    static void clear_instances();

    template<typename T>
    static T* get(const std::string& name)
    {
        auto it = systems_.find(name);
        if (it != systems_.end()) {
            return static_cast<T*>(systems_[name]);
        } else {
            return nullptr;
        }
    }

private:

    static std::unordered_map<std::string, system*> systems_;

};

}
