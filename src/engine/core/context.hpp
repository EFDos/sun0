/*************************************************************************/
/*  context.hpp                                                          */
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
#include "system/system.hpp"

#include <string>
#include <unordered_map>

namespace sun {

class Component;

class SUN_API Context
{
public:

    Context();

    Context(const Context&) = delete;

    Context(Context&&) = delete;

    ~Context();

    void init_systems();

    void shutdown_systems();

    template<typename T>
    T* register_system()
    {
        System* sys = register_system_(T::get_static_type_name());
        return static_cast<T*>(sys);
    }

    template<typename T>
    T* get_system()
    {
        auto* sys = get_system_(T::get_static_type_hash());
        return static_cast<T*>(sys);
    }

    template<typename T>
    T* create_component(uint id = 0, bool init = true)
    {
        for (auto sys : systems_) {
            if (sys.second->handles_component<T>()) {
                return sys.second->create_component<T>(id, init);
            }
        }
        return nullptr;
    }

    template<typename T>
    T* create_resource()
    {
        for (auto sys : systems_) {
            if (sys.second->handles_resource<T>()) {
                return sys.second->create_resource<T>();
            }
        }
        return nullptr;
    }

    Context& operator=(const Context&) = delete;

    Context& operator=(Context&&) = delete;

private:

    System* register_system_(const std::string& type);

    System* get_system_(size_t);

    std::unordered_map<size_t, System*> systems_;
};

}
