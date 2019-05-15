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

class SUN_API context
{
public:

    context();

    context(const context&) = delete;

    context(context&&) = delete;

    ~context();

    void init_systems();

    void shutdown_systems();

    void update_systems();

    template<typename T>
    T* register_system()
    {
        system* sys = register_system_(T::get_static_type_name());
        return static_cast<T*>(sys);
    }

    template<typename T>
    T* get_system()
    {
        auto* sys = get_system_(T::get_static_type_hash());
        return static_cast<T*>(sys);
    }

    context& operator=(const context&) = delete;

    context& operator=(context&&) = delete;

private:

    system* register_system_(const std::string& type);

    system* get_system_(size_t);

    std::unordered_map<size_t, system*> systems_;
};

}
