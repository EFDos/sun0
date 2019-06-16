/*************************************************************************/
/*  rectangle.hpp                                                        */
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

#include "primitive_shape.hpp"

namespace sun {
namespace shapes {

class SUN_API Rectangle : public Shape
{
public:

    Rectangle() : Shape()
    {
        type_ = ShapeType::rectangle;
    }

    Rectangle(float x, float y) : Shape(), size_(x, y)
    {
        type_ = ShapeType::rectangle;
    }

    Rectangle(const Vector2f& size) : Shape(), size_(size)
    {
        type_ = ShapeType::Rectangle;
    }

    Rectangle(const Rectangle&) = default;

    Rectangle(Rectangle&&) = default;

    Rectangle& operator =(const Rectangle& other)
    {
        size_ = other.size_;

        return *this;
    }

    Rectangle& operator =(Rectangle&& other)
    {
        size_ = other.size_;

        return *this;
    }

    ~Rectangle() {}

    inline void set_size(const Vector2f& size)
    {
        size_ = size;
    }

    inline void set_size(float x, float y)
    {
        size_.x = x;
        size_.y = y;
    }

    inline Vector2f get_point(size_t i) const override
    {
        switch(i)
        {
            case 0:
                return {0, 0};
                break;
            case 1:
                return {0, size_.y};
                break;
            case 2:
                return {size_.x, size_.y};
                break;
            case 3:
                return {size_.x, 0};
                break;
            default:
                return {0, 0};
        }
    }

    inline size_t get_point_count() const override
    {
        return 4;
    }

    inline const Vector2f& get_size() const
    {
        return size_;
    }

private:

    Vector2f    size_;
};

}
}
