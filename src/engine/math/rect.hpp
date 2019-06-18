/*************************************************************************/
/*  Rect.hpp                                                             */
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
struct Rect
{
    T x, y, w, h;

    Rect(T p_x = 0, T p_y = 0, T p_w = 0, T p_h = 0) noexcept
    :   x(p_x), y(p_y), w(p_w), h(p_h) {}

    Rect(const Vector2<T>& pos, const Vector2<T>& size)
    :   x(pos.x), y(pos.y), w(size.w), h(size.h) {}

    Rect(const Rect& r) noexcept = default;

    Rect(Rect&&) noexcept = default;

    void set_point(const Vector2<T> point) {
        set_point(point.x, point.y);
    }

    void set_point(T p_x, T p_y) {
        x = p_x;
        y = p_y;
    }

    void set_size(const Vector2<T>& size) {
        set_size(size.x, size.y);
    }

    void set_size(T p_w, T p_h) {
        w = p_w;
        h = p_h;
    }

    inline bool contains(T px, T py)
    {
        return ((px > x && px < x + w) && (py > y && py < y + h));
    }

    inline bool contains(const Vector2<T>& p)
    {
        return ((p.x > x && p.x < x + w) && (p.y > y && p.y < y + h));
    }

    inline bool intersects(const Rect& r)
    {
        return (contains(r.x, r.y) || contains(r.x + r.w, r.y) ||
                contains(r.x + r.w, r.y + r.h) || contains(r.x, r.y + r.h));
    }

    inline Vector2<T> get_position() const
    {
        return Vector2<T>(x,y);
    }

    inline Vector2<T> get_size() const
    {
        return Vector2<T>(w, h);
    }

    Rect& operator=(Rect&&) noexcept = default;

    Rect& operator=(const Rect&) noexcept = default;

    inline static Rect<float> to_rectf(const Rect<int>& r)
    {
        return {static_cast<float>(r.x),
                static_cast<float>(r.y),
                static_cast<float>(r.w),
                static_cast<float>(r.x)};
    }

    inline static Rect<float> to_rectf(const Rect<unsigned>& r)
    {
        return {static_cast<float>(r.x),
                static_cast<float>(r.y),
                static_cast<float>(r.w),
                static_cast<float>(r.x)};
    }
};

using Recti = Rect<int>;
using Rectu = Rect<unsigned>;
using Rectf = Rect<float>;

}
