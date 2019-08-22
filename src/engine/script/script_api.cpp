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

#include "common/types.hpp"

#include "scene/entity.hpp"
#include "scene/animation.hpp"

#include "graphics/sprite.hpp"

#include "physics/physics_server.hpp"
#include "physics/rigid_body.hpp"
#include "physics/raycast.hpp"

namespace sun {

void ScriptContext::register_api(sol::state& state)
{
    sol::table keyboard_table = state.create_named_table("Keyboard");

    state.new_enum("Key",
        "R", keyboard::Key::R,
        "Up", keyboard::Key::Up,
        "Left", keyboard::Key::Left,
        "Right", keyboard::Key::Right,
        "Down", keyboard::Key::Down);

    keyboard_table.set_function("is_key_pressed", keyboard::is_key_pressed);

    state.new_enum("EventType",
        "KeyPressed", EventType::KeyPressed,
        "KeyReleased", EventType::KeyReleased);

    state.new_usertype<Event>("Event",
        "type", &Event::type);

    state.new_usertype<Time>("Time",
        sol::constructors<void(int64), void(void)>(),
        "as_seconds", &Time::as_seconds,
        "as_microseconds", &Time::as_microseconds,
        "as_milliseconds", &Time::as_milliseconds);

    state.set_function("seconds", &Time::seconds);
    state.set_function("microseconds", &Time::microseconds);
    state.set_function("milliseconds", &Time::milliseconds);

    state.new_usertype<Vector2f>("Vector2",
        sol::constructors<Vector2f(float, float), Vector2f(const Vector2f&)>(),
        "x", &Vector2f::x,
        "y", &Vector2f::y);

    state.new_usertype<Entity>("Entity",
        "set_position", sol::resolve<void(float, float)>(&Entity::set_position),
        "get_position", &Entity::get_position,
        "get_name", &Entity::get_name,
        "get_component", &Entity::get_component<Component>,
        "get_rigid_body", &Entity::get_component<RigidBody>,
        "get_raycast", &Entity::get_component<Raycast>,
        "get_animation", &Entity::get_component<Animation>,
        "get_sprite", &Entity::get_component<Sprite>);

    state.new_usertype<Animation>("Animation",
        "create_track", &Animation::create_track);

    //state.new_usertype<AnimationTrack>("AnimationTrack",
    //    "insert_key", &AnimationTrack::insert_key);

    //state.new_usertype<AnimationTrack::KeyFrame>("KeyFrame",
    //    sol::constructors<Vector2f, >());

    state.new_usertype<Sprite>("Sprite",
        "set_frame", &Sprite::set_frame,
        "set_frames", &Sprite::set_frames);

    state.new_usertype<RigidBody>("RigidBody",
        "move_to_entity", &RigidBody::move_to_entity,
        "apply_linear_impulse", &RigidBody::apply_linear_impulse,
        "get_linear_velocity", &RigidBody::get_linear_velocity,
        "get_angular_velocity", &RigidBody::get_angular_velocity);

    state.new_usertype<Raycast>("Raycast",
        "cast_to", &Raycast::cast_to,
        "is_colliding", &Raycast::is_colliding,
        "get_collision", &Raycast::get_collision);

    state.new_usertype<physics::RaycastCollision>("RaycastCollision",
        "point", &physics::RaycastCollision::point,
        "normal", &physics::RaycastCollision::normal,
        "owner", &physics::RaycastCollision::owner);
}

}
