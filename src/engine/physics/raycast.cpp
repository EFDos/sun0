/*************************************************************************/
/*  raycast.hpp                                                          */
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
#include "raycast.hpp"
#include "physics_server.hpp"
#include "scene/entity.hpp"

namespace sun {

Raycast::Raycast(Context& context)
:   Component(context),
    physics_server_ptr_(nullptr)
{}

void Raycast::init()
{
    physics_server_ptr_ = context_.get_system<PhysicsServer>();

    Component::init();
}

bool Raycast::is_colliding() const
{
    if (physics_server_ptr_ != nullptr) {
        Vector2f pos;
        if (owning_entity_ != nullptr) {
            pos = owning_entity_->get_position();
        }
        return physics_server_ptr_->lazy_raycast(pos, pos + cast_point_);
    }
    return false;
}

physics::RaycastCollision Raycast::get_collision() const
{
    if (physics_server_ptr_ != nullptr) {
        Vector2f pos;
        if (owning_entity_ != nullptr) {
            pos = owning_entity_->get_position();
        }
        return physics_server_ptr_->raycast(pos, pos + cast_point_);
    }
    return physics::RaycastCollision();
}

}
