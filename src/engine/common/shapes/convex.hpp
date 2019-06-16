/*************************************************************************/
/*  convex.hpp                                                           */
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
#include <vector>

namespace sun {
namespace shapes {

class SUN_API Convex : public Shape
{
public:

    Convex() : Shape()
    {
        type_ = ShapeType::Convex;
    }

    Convex(size_t size) : Shape(), vertices_(size)
    {
        type_ = ShapeType::Convex;
    }

    Convex(const std::initializer_list<vector2f>& vertices)
    : Shape(), vertices_(vertices)
    {
        type_ = ShapeType::Convex;
    }

    Convex(const Convex& convex) = default;

    Convex(Convex&& convex) = default;

    Convex& operator =(const Convex& other)
    {
        vertices_ = other.vertices_;

        return *this;
    }

    Convex& operator =(Convex&& other)
    {
        vertices_ = std::move(other.vertices_);

        return *this;
    }

    ~Convex() {}

    inline void set_point(size_t i, const vector2f& point)
    {
        if (i > vertices_.size())
            return;

        vertices_[i] = point;
    }

    inline void set_point_count(size_t points)
    {
        vertices_.resize(points);
    }

    inline vector2f get_point(size_t i) const override
    {
        return vertices_[i];
    }

    inline size_t get_point_count() const override
    {
        return vertices_.size();
    }

private:

    std::vector<vector2f>   vertices_;
};

}
}
