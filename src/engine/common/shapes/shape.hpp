/*************************************************************************/
/*  shape.hpp                                                  */
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

#include "common/config.hpp"
#include "math/vector2.hpp"

namespace sun {
namespace shapes {

enum class ShapeType
{
    Rectangle,
    Circle,
    Convex
};

class SUN_API Shape
{
public:

    Shape() {}

    virtual ~Shape() {}

    virtual Vector2f get_point(size_t index) const = 0;

    virtual size_t get_point_count() const = 0;

    ShapeType get_type() const
    {
        return type_;
    }

protected:
    ShapeType    type_;
};

}
}
