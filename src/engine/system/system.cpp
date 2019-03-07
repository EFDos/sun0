/*************************************************************************/
/*  system.cpp                                                           */
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
#include "system.hpp"
#include "core/logger.hpp"

namespace sun {

std::unordered_map<std::string, system*> system::systems_;

system::system()
{
}

system::~system()
{
    for (auto sys : systems_)
    {
        if (sys.second == this) {
            sys.second = nullptr;
        }
    }
}

void system::clear_instances()
{
    for (auto it = systems_.begin() ; it != systems_.end() ;
        it = systems_.erase(it))
    {
        if (it->second != nullptr) {
            sun_logf_info("*-- System Shutdown: %s --*", it->second->get_name());
            it->second->shutdown();
            delete it->second;
        }
    }
}

void system::register_instance(system* instance)
{
    auto it = systems_.find(instance->get_name());

    if (it == systems_.end()) {
        sun_logf_info("*-- System Init: %s --*", instance->get_name());
        instance->init();
        systems_[instance->get_name()] = instance;
    } else {
        sun_logf_error("System with name %s is already registered.",
            instance->get_name());

        delete instance;
    }
}

}
