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

class SUN_API rectangle : public primitive_shape
{
public:

    rectangle() : primitive_shape()
    {
        type_ = type::rectangle;
    }

    rectangle(float x, float y) : primitive_shape(), size_(x, y)
    {
        type_ = type::rectangle;
    }

    rectangle(const vector2f& size) : primitive_shape(), size_(size)
    {
        type_ = type::rectangle;
    }

    rectangle(const rectangle&) = default;

    rectangle(rectangle&&) = default;

    rectangle& operator =(const rectangle& other)
    {
        size_ = other.size_;

        return *this;
    }

    rectangle& operator =(rectangle&& other)
    {
        size_ = other.size_;

        return *this;
    }

    ~rectangle() {}

    inline void set_size(const vector2f& size)
    {
        size_ = size;
    }

    inline void set_size(float x, float y)
    {
        size_.x = x;
        size_.y = y;
    }

    inline vector2f get_point(size_t i) const override
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

    inline const vector2f& get_size() const
    {
        return size_;
    }

private:

    vector2f    size_;
};

}
}
