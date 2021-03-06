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
struct BaseColor
{
    T r;
    T g;
    T b;
    T a;

    constexpr BaseColor(T p_r = 0, T p_g = 0, T p_b = 0, T p_a = 255) noexcept
    : r(p_r), g(p_g), b(p_b), a(p_a) {}

    BaseColor(const BaseColor&) noexcept = default;

    BaseColor(BaseColor&&) noexcept = default;

    inline void set(T p_r, T p_g, T p_b, T p_a) {
        r = p_r;
        g = p_g;
        b = p_b;
        a = p_a;
    }

    BaseColor<T>& operator=(const BaseColor&) noexcept = default;

    BaseColor<T>& operator=(BaseColor&&) noexcept = default;

    inline bool operator==(const BaseColor<T>& other) const
    {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    inline bool operator!=(const BaseColor<T>& other) const
    {
        return r != other.r || g != other.g || b != other.b || a != other.a;
    }

    inline BaseColor<T> operator+=(const BaseColor<T>& other)
    {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;
        return *this;
    }

    inline BaseColor<T> operator+=(T v)
    {
        r += v;
        g += v;
        b += v;
        a += v;
        return *this;
    }

    inline BaseColor<T> operator-=(const BaseColor<T>& other)
    {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;
        return *this;
    }

    inline BaseColor<T> operator-=(T v)
    {
        r -= v;
        g -= v;
        b -= v;
        a -= v;
        return *this;
    }

    inline BaseColor<T> operator*(const BaseColor<T>& other)
    {
        return {r * other.r, g * other.g, b * other.b, a * other.a};
    }

    inline BaseColor<T> operator*(T v)
    {
        return {r * v, g * v, b * v, a * v};
    }

    inline BaseColor<T> operator*=(const BaseColor<T>& other)
    {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;
        return *this;
    }

    inline BaseColor<T> operator*=(T v)
    {
        r *= v;
        g *= v;
        b *= v;
        a *= v;
        return *this;
    }

    inline BaseColor<T> operator/=(const BaseColor<T>& other)
    {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;
        return *this;
    }

    inline BaseColor<T> operator/=(T v)
    {
        r /= v;
        g /= v;
        b /= v;
        a /= v;
        return *this;
    }

    inline static BaseColor<uint8> to_color(const BaseColor<float>& cf)
    {
        return {static_cast<uint8>(cf.r * 255.f),
                static_cast<uint8>(cf.g * 255.f),
                static_cast<uint8>(cf.b * 255.f),
                static_cast<uint8>(cf.a * 255.f)};
    }

    inline static BaseColor<float> to_colorf(const BaseColor<uint8>& c)
    {
        return {static_cast<float>(c.r / 255.f),
                static_cast<float>(c.g / 255.f),
                static_cast<float>(c.b / 255.f),
                static_cast<float>(c.a / 255.f)};
    }

    static const BaseColor<uint8> RED;
    static const BaseColor<uint8> BLUE;
    static const BaseColor<uint8> GREEN;
    static const BaseColor<uint8> BLACK;
    static const BaseColor<uint8> WHITE;
    static const BaseColor<uint8> YELLOW;
    static const BaseColor<uint8> MAGENTA;
    static const BaseColor<uint8> SUN;
    static const BaseColor<uint8> LIGHT_GREY;
    static const BaseColor<uint8> DARK_GREY;
};

template<typename T>
BaseColor<T> operator+(const BaseColor<T> &a, const BaseColor<T>& b)
{
    BaseColor<T> col(a);
    return col += b;
}

template<typename T>
BaseColor<T> operator-(const BaseColor<T> &a, const BaseColor<T>& b)
{
    BaseColor<T> col(a);
    return col -= b;
}

template<typename T>
BaseColor<T> operator*(const BaseColor<T> &a, const BaseColor<T>& b)
{
    BaseColor<T> col(a);
    return col *= b;
}

template<typename T>
BaseColor<T> operator/(const BaseColor<T> &a, const BaseColor<T>& b)
{
    BaseColor<T> col(a);
    return col /= b;
}

using Color     = BaseColor<uint8>;
using Colorf    = BaseColor<float>;

}
