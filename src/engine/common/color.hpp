/*************************************************************************/
/*  color.hpp                                                            */
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

#include "int.hpp"

namespace sun {

template<typename T>
struct base_color
{
    T r;
    T g;
    T b;
    T a;

    base_color(T p_r = 0, T p_g = 0, T p_b = 0, T p_a = 0) noexcept
    : r(p_r), g(p_g), b(p_b), a(p_a) {}

    base_color(const base_color&) noexcept = default;

    base_color(base_color&&) noexcept = default;

    void set(T p_r, T p_g, T p_b, T p_a) {
        r = p_r;
        g = p_g;
        b = p_b;
        a = p_a;
    }

    base_color<T>& operator=(const base_color&) noexcept = default;

    base_color<T>& operator=(base_color&&) noexcept = default;

    bool operator==(const base_color<T>& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const base_color<T>& other) const
    {
        return r != other.r || g != other.g || b != other.b || a != other.a;
    }

    base_color<T> operator+=(const base_color<T>& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    base_color<T> operator+=(T v)
    {
        r += v;
        g += v;
        b += v;
        a += v;
    }

    base_color<T> operator-=(const base_color<T>& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }

    base_color<T> operator-=(T v)
    {
        r -= v;
        g -= v;
        b -= v;
        a -= v;
    }

    base_color<T> operator*=(const base_color<T>& other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;
    }

    base_color<T> operator*=(T v)
    {
        r *= v;
        g *= v;
        b *= v;
        a *= v;
    }

    base_color<T> operator/=(const base_color<T>& other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
        return *this;
    }

    base_color<T> operator/=(T v)
    {
        r /= v;
        g /= v;
        b /= v;
        a /= v;
    }
};

template<typename T>
base_color<T> operator+(const base_color<T> &a, const base_color<T>& b)
{
    base_color<T> col(a);
    return col += b;
}

template<typename T>
base_color<T> operator-(const base_color<T> &a, const base_color<T>& b)
{
    base_color<T> col(a);
    return col -= b;
}

template<typename T>
base_color<T> operator*(const base_color<T> &a, const base_color<T>& b)
{
    base_color<T> col(a);
    return col *= b;
}

template<typename T>
base_color<T> operator/(const base_color<T> &a, const base_color<T>& b)
{
    base_color<T> col(a);
    return col /= b;
}

using color     = base_color<uint8>;
using colorf    = base_color<float>;

inline color to_color(const colorf& cf)
{
    return color(static_cast<uint8>(cf.r * 255.f),
                 static_cast<uint8>(cf.g * 255.f),
                 static_cast<uint8>(cf.b * 255.f),
                 static_cast<uint8>(cf.a * 255.f));
}

inline colorf to_colorf(const color& c)
{
    return colorf(static_cast<float>(c.r / 255.f),
                  static_cast<float>(c.g / 255.f),
                  static_cast<float>(c.b / 255.f),
                  static_cast<float>(c.a / 255.f));
}

}
