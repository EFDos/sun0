/*************************************************************************/
/*  rigid_body.cpp                                                       */
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
#include "rigid_body.hpp"
#include "physics_server.hpp"

#include "common/shapes/primitive_shape.hpp"
#include "common/shapes/rectangle.hpp"
#include "common/shapes/circle.hpp"
#include "common/shapes/convex.hpp"
#include "core/context.hpp"
#include "core/logger.hpp"
#include "scene/entity.hpp"

#include <Box2D/Box2D.h>

namespace sun {

rigid_body::rigid_body(context& p_context)
:   component(p_context),
    type_(type::undefined),
    body_(nullptr)
{
}

void rigid_body::create(const shapes::primitive_shape& shape, type t)
{
    if (t == type::undefined) {
        sun_log_error("Can't create shape of type 'undefined'");
    }

    type_ = t;
    b2BodyDef body_def;

    switch (t)
    {
        case type::sensor: // falls into static_body
        case type::static_body:
            body_def.type = b2_staticBody;
            break;
        case type::dynamic_body:
            body_def.type = b2_dynamicBody;
            break;
        case type::kinematic_body:
            body_def.type = b2_kinematicBody;
            break;
        default:
            return;
    }

    if (owning_entity_ != nullptr) {
        body_def.position = physics::to_b2vec(owning_entity_->get_position());
    }

    auto world = context_.get_system<physics_server>()->get_b2_world();

    body_ = world->CreateBody(&body_def);

    if (body_ == nullptr) {
        sun_log_error("Failed to create rigid body");
    }


    switch (shape.get_type())
    {
        case shapes::type::rectangle:
            {
                auto rectangle = static_cast<const shapes::rectangle&>(shape);
                b2PolygonShape b2_shape;
                b2_shape.SetAsBox(
                    physics::scale_to_meters(rectangle.get_size().x) / 2,
                    physics::scale_to_meters(rectangle.get_size().y) / 2
                );
                body_->CreateFixture(&b2_shape, 1.f);
            }
            break;
        case shapes::type::circle:
            {
                auto circle = static_cast<const shapes::circle&>(shape);
                b2CircleShape b2_shape;
                b2_shape.m_radius = physics::scale_to_meters(circle.get_radius());
                body_->CreateFixture(&b2_shape, 1.f);
            }
            break;
        case shapes::type::convex:
            {
                b2PolygonShape b2_shape;
                b2Vec2* b2_vec_list = new b2Vec2[shape.get_point_count()];

                for (uint i = 0 ; i < shape.get_point_count() ; ++i) {
                    b2_vec_list[i] = physics::to_b2vec(shape.get_point(i));
                }

                b2_shape.Set(b2_vec_list, shape.get_point_count());

                body_->CreateFixture(&b2_shape, 1.f);

                delete [] b2_vec_list;
            }
            break;
    }
}

void rigid_body::update_entity()
{
    if (owning_entity_ == nullptr || body_ == nullptr) {
        return;
    }

    owning_entity_->set_position(physics::to_vec2(body_->GetPosition()));
    owning_entity_->set_rotation(math::rad_to_deg(body_->GetAngle()));
}

}
