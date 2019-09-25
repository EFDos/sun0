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
#include "component.hpp"

#ifdef SUN_PLATFORM_HAIKU
#include <algorithm>
#endif

namespace sun {

System::System(Context& context) : context_(context), initialized_(false)
{}

System::~System()
{}

bool System::init()
{
    return initialized_ = true;
}

void System::shutdown()
{
    components_.clear();
    initialized_ = false;
}

void System::update(float delta)
{}

void System::handle_events(Event& event)
{}

void System::clear_components()
{
    components_.erase(
        std::remove_if(components_.begin(), components_.end(), [] (const Ref<Component>& comp) {
            return comp->is_terminal();
        }), components_.end()
    );
}

Resource* System::create_resource_(uint type_hash)
{
    return nullptr;
}

bool System::handles_resource_(uint type_hash)
{
    return false;
}

}
