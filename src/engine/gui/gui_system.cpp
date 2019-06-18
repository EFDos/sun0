/*************************************************************************/
/*  gui_system.cpp                                                       */
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
#include "gui_system.hpp"

#include "core/logger.hpp"

namespace sun {

GUISystem::GUISystem(Context& context)
:   System(context)
{}

bool GUISystem::init()
{
    sun_log_info("GUI ready");
    return true;
}

void GUISystem::shutdown()
{
}

Component* GUISystem::create_component_(uint type_hash, uint id)
{
    return nullptr;
}

bool GUISystem::handles_component_(uint type_hash)
{
    return false;
}

}
