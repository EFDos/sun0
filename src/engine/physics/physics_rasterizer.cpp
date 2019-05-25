/*************************************************************************/
/*  physics_rasterizer.cpp                                               */
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
#include "physics_rasterizer.hpp"
//#include "common/shapes/rectangle.hpp"
#include "common/shapes/circle.hpp"
//#include "common/shapes/convex.hpp"
#include "graphics/renderer.hpp"

#include <vector>

namespace sun {

physics_rasterizer::physics_rasterizer(uint scale)
:   scale_(scale)
{
}

void physics_rasterizer::DrawCircle(const b2Vec2& center, float32 radius,
    const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    vector2f vertices[30];

    shapes::circle circle;
    circle.set_radius(radius * scale_);
    circle.set_point_count(30);

    for(int i = 0 ; i < 30 ; ++i)
    {
        const vector2f& point = circle.get_point(i);
        vertices[i].x = point.x + (center.x - radius) * scale_;
        vertices[i].y = point.y + (center.y + radius) * scale_ * -1;
    }

    //renderer_->draw_primitive(30, vertices, color::white);
}

}
