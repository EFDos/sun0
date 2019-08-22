/*************************************************************************/
/*  rigid_body.hpp                                                       */
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
#pragma once

#include "math/vector2.hpp"
#include "system/component.hpp"

class b2Body;

namespace sun {

namespace shapes {
class Shape;
}

class SUN_API RigidBody final : public Component
{
public:

    SUN_COMPONENT_TYPE(RigidBody)

    enum class Type
    {
        Undefined,
        Static,
        Dynamic,
        Kinematic,
        Sensor
    };

    RigidBody(Context&);

    void create(const shapes::Shape& shp, Type t);

    void update(float delta) override;

    void apply_linear_impulse(const Vector2f& impulse);

    void apply_linear_impulse(const Vector2f& impulse,
                              const Vector2f& point);

    void apply_angular_impulse(float impulse);

    void move_to_entity();

    void set_angular_velocity(float vel);

    void set_restitution(float restitution);

    void set_friction(float friction);

    void set_density(float density);

    Vector2f get_linear_velocity() const;

    float get_angular_velocity() const;

    inline Type get_type() const {
        return type_;
    }

private:

    Type    type_;
    b2Body* body_;
};

}
