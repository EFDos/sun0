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
#include "rigid_body.hpp"

#include "core/logger.hpp"

namespace sun {

namespace physics {

uint pixel_scale = PhysicsServer::DEFAULT_METER;

uint get_pixel_scale() {
    return pixel_scale;
}

}

PhysicsServer::PhysicsServer(Context& context)
:   System(context),
    world_(b2Vec2(0.f, -9.807f)),
    debug_rasterizer_(DEFAULT_METER),
    contact_listener_(),
    timestep_(1/45.f),
    vel_iterations_(8), //These are the suggested values for velocity
    pos_iterations_(3), //and position iterations from the Box2D manual
    debug_draw_(false)
{
    world_.SetDebugDraw(&debug_rasterizer_);
    world_.SetContactListener(&contact_listener_);

    debug_rasterizer_.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
}

bool PhysicsServer::init()
{
    sun_log_info("Box2D World created");
    sun_log_info("Physics Server ready.");
    return System::init();
}

void PhysicsServer::shutdown()
{
    for (auto body : bodies_) {
        delete body;
    }

    bodies_.clear();
    System::shutdown();
    sun_log_info("Physics System shutdown.");
}

void PhysicsServer::update(float delta)
{
    world_.Step(timestep_, vel_iterations_, pos_iterations_);
    for (auto body : bodies_) {
        if (body->get_update()) {
            body->update(delta);
        }
    }
}

void PhysicsServer::draw_physics_debug(Renderer* render)
{
    if (!debug_draw_) {
        return;
    }

    if (debug_rasterizer_.get_renderer() == nullptr) {
        debug_rasterizer_.set_renderer(render);
    }

    world_.DrawDebugData();
}

bool PhysicsServer::lazy_raycast(const Vector2f& begin, const Vector2f& end)
{
    LazyRaycastCallback callback;
    world_.RayCast(&callback, physics::to_b2vec(begin), physics::to_b2vec(end));

    return callback.is_colliding();
}

physics::RaycastCollision PhysicsServer::raycast(const Vector2f& begin,
                                                   const Vector2f& end)
{
    physics::RaycastCollision collision_obj;
    NearestRaycastCallback callback(collision_obj);
    world_.RayCast(&callback, physics::to_b2vec(begin), physics::to_b2vec(end));

    return collision_obj;
}

void PhysicsServer::set_gravity(const Vector2f& gravity)
{
    world_.SetGravity(b2Vec2(gravity.x, gravity.y));
}

void PhysicsServer::set_pixel_scale(uint scale)
{
    if (scale == 0) {
        physics::pixel_scale = DEFAULT_METER;
        return;
    }

    physics::pixel_scale = scale;
    debug_rasterizer_.set_scale(scale);
}

void PhysicsServer::set_update_rate(float timestep, int vel_it, int pos_it)
{
    timestep_ = 1 / timestep;
    vel_iterations_ = vel_it;
    pos_iterations_ = pos_it;
}

Component* PhysicsServer::create_component_(uint type_hash, uint id)
{
    Component* comp = nullptr;
    if (type_hash == RigidBody::get_static_type_hash()) {
        comp = new RigidBody(context_);
        bodies_.push_back(static_cast<RigidBody*>(comp));
    }
    comp->set_id(id);
    return comp;
}

bool PhysicsServer::handles_component_(uint type_hash)
{
    if (type_hash == RigidBody::get_static_type_hash())
    {
        return true;
    }
    return false;
}

}
