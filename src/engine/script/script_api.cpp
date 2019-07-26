/*************************************************************************/
/*  script_api.cpp                                                       */
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
#include "script_context.hpp"
#include "core/event.hpp"
#include "scene/entity.hpp"

#include "common/types.hpp"

#include "physics/physics_server.hpp"
#include "physics/rigid_body.hpp"

namespace sun {

void ScriptContext::register_api(sol::state& state)
{
    //state.new_usertype<Event>("Event")

    sol::table keyboard_table = state.create_named_table("Keyboard");

    state.new_enum("Key",
        "R", keyboard::Key::R,
        "Up", keyboard::Key::Up,
        "Left", keyboard::Key::Left,
        "Right", keyboard::Key::Right,
        "Down", keyboard::Key::Down);

    keyboard_table.set_function("is_key_pressed", keyboard::is_key_pressed);

    state.new_usertype<Vector2f>("Vector2",
        sol::constructors<Vector2f(float, float), Vector2f(const Vector2f&)>(),
        "x", &Vector2f::x,
        "y", &Vector2f::y);

    state.new_usertype<Entity>("Entity",
        "set_position", sol::resolve<void(float, float)>(&Entity::set_position),
        "get_name", &Entity::get_name,
        "get_component", &Entity::get_component<Component>,
        "get_rigid_body", &Entity::get_component<RigidBody>);

    state.new_usertype<RigidBody>("RigidBody",
        "move_to_entity", &RigidBody::move_to_entity,
        "apply_linear_impulse", &RigidBody::apply_linear_impulse);

    state.new_usertype<physics::RaycastCollision>("RaycastCollision",
        "point", &physics::RaycastCollision::point,
        "normal", &physics::RaycastCollision::normal,
        "owner", &physics::RaycastCollision::owner);
}

}
