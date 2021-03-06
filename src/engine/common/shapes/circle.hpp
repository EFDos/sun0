/*************************************************************************/
/*  circle.hpp                                                           */
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

#include "shape.hpp"
#include "math/math.hpp"


namespace sun {
namespace shapes {

class SUN_API Circle : public Shape
{
public:

    Circle() : Shape(), radius_(1.f), points_(360)
    {
        type_ = ShapeType::Circle;
    }

    Circle(float radius, size_t points) : Shape(), radius_(radius),
        points_(points)
    {
        type_ = ShapeType::Circle;
    }

    Circle(const Circle& circle) = default;

    Circle(Circle&& circle) = default;

    Circle& operator =(const Circle& circle)
    {
        radius_ = circle.radius_;
        points_ = circle.points_;

        return *this;
    }

    Circle& operator =(Circle&& circle)
    {
        radius_ = circle.radius_;
        points_ = circle.points_;

        return *this;
    }

    ~Circle() {}

    inline void set_point_count(size_t points)
    {
        points_ = points;
    }

    inline void set_radius(float radius)
    {
        radius_ = radius;
    }

    inline Vector2f get_point(size_t i) const override
    {
        float angle = i * 2 * math::PI / points_ - math::PI / 2;
        float x = std::cos(angle) * radius_;
        float y = std::sin(angle) * radius_;

        return {radius_ + x, radius_ + y};
    }

    inline size_t get_point_count() const override
    {
        return points_;
    }

    inline float get_radius() const
    {
        return radius_;
    }

private:

    float   radius_;
    size_t  points_;

};

}
}
