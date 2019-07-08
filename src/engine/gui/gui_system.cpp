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

#include "core/context.hpp"
#include "core/logger.hpp"
//#include "core/event.hpp"
#include "graphics/renderer.hpp"

namespace sun {

GUISystem::GUISystem(Context& context)
:   System(context),
    default_theme_(Color(40, 40, 60), Color(80, 80, 120)),
    frame_(context, {800, 600})
{
    frame_.set_gui_system(this);
}

bool GUISystem::init()
{
    sun_log_info("GUISystem ready");
    return true;
}

void GUISystem::shutdown()
{
}

void GUISystem::render(Renderer* renderer)
{
    //renderer->set_camera_transform(Matrix4());

    frame_.draw(renderer);
}

void GUISystem::handle_events(const Event& event)
{
    frame_.handle_events(event);
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
