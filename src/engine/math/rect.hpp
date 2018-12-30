/*************************************************************************/
/*  rect.hpp                                                             */
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

#include "vector2.hpp"

namespace sun {

template<typename T>
struct rect
{
    T x, y, w, h;

    rect(T p_x = 0, T p_y = 0, T p_w = 0, T p_h = 0) noexcept
    :   x(p_x), y(p_y), w(p_w), h(p_h) {}

    rect(const vector2<T>& pos, const vector2<T>& size)
    :   x(pos.x), y(pos.y), w(size.w), h(size.h) {}

    rect(const rect& r) noexcept = default;

    rect(rect&&) noexcept = default;

    inline bool contains(T px, T py)
    {
        return ((px > x && px < x + w) && (py > y && py < y + h));
    }

    inline bool contains(const vector2<T>& p)
    {
        return ((p.x > x && p.x < x + w) && (p.y > y && p.y < y + h));
    }

    inline bool intersects(const rect& r)
    {
        return (contains(r.x) || contains(r.y) ||
                contains(r.w) || contains(r.h));
    }

    inline vector2<T> get_position() const
    {
        return vector2<T>(x,y);
    }

    inline vector2<T> get_size() const
    {
        return vector2<T>(w, h);
    }

    rect& operator=(rect&&) noexcept = default;

    rect& operator=(const rect&) noexcept = default;
};

}
