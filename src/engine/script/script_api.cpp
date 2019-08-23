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
        "KeyReleased", EventType::KeyReleased,
        "TextEntered", EventType::TextEntered,
        "MouseWheelScrolled", EventType::MouseWheelScrolled,
        "MouseButtonPressed", EventType::MouseButtonPressed,
        "MouseButtonReleased", EventType::MouseButtonReleased,
        "MouseMoved", EventType::MouseMoved,
        "MouseEntered", EventType::MouseEntered,
        "MouseLeft", EventType::MouseLeft,
        "JoystickButtonPressed", EventType::JoystickButtonPressed,
        "JoystickButtonReleased", EventType::JoystickButtonReleased,
        "JoystickHatMoved", EventType::JoystickHatMoved,
        "JoystickAxisMoved", EventType::JoystickAxisMoved,
        "JoystickConnected", EventType::JoystickConnected,
        "JoystickDisconnected", EventType::JoystickDisconnected,
        "TouchBegan", EventType::TouchBegan,
        "TouchMoved", EventType::TouchMoved,
        "TouchEnded", EventType::TouchEnded);

    state.new_usertype<Event>("Event",
        "type", &Event::type,
        "key", &Event::key,
        "text_input", &Event::text_input,
        "mouse_move", &Event::mouse_move,
        "mouse_button", &Event::mouse_button,
        "mouse_wheel", &Event::mouse_wheel,
        "joystick_axis", &Event::joystick_axis,
        "joystick_hat", &Event::joystick_hat,
        "joystick_button", &Event::joystick_button,
        "joystick_connect", &Event::joystick_connect,
        "touch", &Event::touch);

    state.new_usertype<Event::KeyEvent>("KeyEvent",
        "code", &Event::KeyEvent::code,
        "alt", &Event::KeyEvent::alt,
        "control", &Event::KeyEvent::control,
        "shift", &Event::KeyEvent::shift,
        "system", &Event::KeyEvent::system);

    state.new_usertype<Event::TextInputEvent>("TextInputEvent",
        "text", &Event::TextInputEvent::text,
        "size", &Event::TextInputEvent::size);

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
        //sol::meta_function::unary_minus, &Vector2f::operator-,
        "x", &Vector2f::x,
        "y", &Vector2f::y);

    state.new_usertype<Entity>("Entity",
        "position", sol::property(&Entity::get_position,
            sol::resolve<void(const Vector2f&)>(&Entity::set_position)),
        "name", sol::property(&Entity::get_name, &Entity::get_name),
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
        "apply_linear_impulse",
            sol::overload(sol::resolve<void(const Vector2f&)>(&RigidBody::apply_linear_impulse),
                          sol::resolve<void(const Vector2f&, const Vector2f&)>(&RigidBody::apply_linear_impulse)),
        "apply_angular_impulse", &RigidBody::apply_angular_impulse,
        "linear_velocity", sol::property(&RigidBody::get_linear_velocity,
            &RigidBody::set_linear_velocity),
        "linear_damping", sol::property(&RigidBody::get_linear_damping,
            &RigidBody::set_linear_damping),
        "angular_velocity", sol::property(&RigidBody::get_angular_velocity,
            &RigidBody::set_angular_velocity),
        "angular_damping", sol::property(&RigidBody::get_angular_damping,
            &RigidBody::set_angular_damping),
        "friction", sol::property(&RigidBody::get_friction,
            &RigidBody::set_friction),
        "restitution", sol::property(&RigidBody::get_restitution,
            &RigidBody::set_restitution),
        "set_density", sol::property(&RigidBody::get_density,
            &RigidBody::set_density));

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
