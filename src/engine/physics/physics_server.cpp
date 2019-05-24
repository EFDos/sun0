/*************************************************************************/
/*  physics_server.cpp                                                   */
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
#include "physics_server.hpp"
#include "core/logger.hpp"

namespace sun {

physics_server::physics_server(context& p_context)
:   system(p_context),
    world_(b2Vec2(0.f, -9.807f))
{
}

bool physics_server::init()
{
    sun_log_info("Box2D World created");
    sun_log_info("Physics System ready.");
    return system::init();
}

void physics_server::shutdown()
{
    system::shutdown();
    sun_log_info("Physics System shutdown.");
}

component* physics_server::create_component_(uint type_hash)
{
    return nullptr;
}

bool physics_server::handles_component_(uint type_hash)
{
    return false;
}

}
