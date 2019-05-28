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

uint pixel_scale = physics_server::default_meter;

uint get_pixel_scale() {
    return pixel_scale;
}

}

physics_server::physics_server(context& p_context)
:   system(p_context),
    world_(b2Vec2(0.f, -9.807f)),
    debug_rasterizer_(default_meter),
    timestep_(1/45.f),
    vel_iterations_(8), //These are the suggested values for velocity
    pos_iterations_(3), //and position iterations from the Box2D manual
    debug_draw_(false)
{
    world_.SetDebugDraw(&debug_rasterizer_);

    debug_rasterizer_.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
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

void physics_server::update()
{
    world_.Step(timestep_, vel_iterations_, pos_iterations_);
    for (auto body : bodies_) {
        body->update_entity();
    }
}

void physics_server::draw_physics_debug(renderer* render)
{
    if (!debug_draw_) {
        return;
    }

    if (debug_rasterizer_.get_renderer() == nullptr) {
        debug_rasterizer_.set_renderer(render);
    }

    world_.DrawDebugData();
}

bool physics_server::lazy_raycast(const vector2f& begin, const vector2f& end)
{
    lazy_raycast_callback callback;
    world_.RayCast(&callback, physics::to_b2vec(begin), physics::to_b2vec(end));

    return callback.is_colliding();
}

physics::raycast_collision physics_server::raycast(const vector2f& begin,
                                                   const vector2f& end)
{
    physics::raycast_collision collision_obj;
    nearest_raycast_callback callback(collision_obj);
    world_.RayCast(&callback, physics::to_b2vec(begin), physics::to_b2vec(end));

    return collision_obj;
}

void physics_server::set_gravity(const vector2f& gravity)
{
    world_.SetGravity(b2Vec2(gravity.x, gravity.y));
}

void physics_server::set_pixel_scale(uint scale)
{
    if (scale == 0) {
        physics::pixel_scale = default_meter;
        return;
    }

    physics::pixel_scale = scale;
    // debug_raster
}

void physics_server::set_update_rate(float timestep, int vel_it, int pos_it)
{
    timestep_ = 1 / timestep;
    vel_iterations_ = vel_it;
    pos_iterations_ = pos_it;
}

component* physics_server::create_component_(uint type_hash, uint id)
{
    component* comp = nullptr;
    if (type_hash == rigid_body::get_static_type_hash()) {
        comp = new rigid_body(context_);
        bodies_.push_back(static_cast<rigid_body*>(comp));
    }
    comp->set_id(id);
    return comp;
}

bool physics_server::handles_component_(uint type_hash)
{
    if (type_hash == rigid_body::get_static_type_hash())
    {
        return true;
    }
    return false;
}

}
