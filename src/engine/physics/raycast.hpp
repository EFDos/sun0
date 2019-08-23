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
#pragma once

#include "math/vector2.hpp"
#include "system/component.hpp"
#include "physics_server.hpp"

namespace sun {

class SUN_API Raycast final : public Component
{
public:

    SUN_COMPONENT_TYPE(Raycast)

    Raycast(Context&);

    void init() override;

    inline void cast_to(const Vector2f& point) {
        cast_point_ = point;
    }

    bool is_colliding() const;

    physics::RaycastCollision get_collision() const;

private:

    Vector2f        cast_point_;
    PhysicsServer*  physics_server_ptr_;
};

}
