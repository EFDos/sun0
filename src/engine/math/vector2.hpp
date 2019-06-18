/*************************************************************************/
/*  vector2.hpp                                                          */
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

#include <cmath>

namespace sun {

template<typename T = float>
class Vector2
{
public:

	union {
		T x;
		T w;
	};

	union {
		T y;
		T h;
	};

	Vector2(T p_x = 0, T p_y = 0) noexcept : x(p_x), y(p_y) {}

	Vector2(const T* data) : x(data[0]), y(data[1]) {}

	Vector2(const Vector2<T>&) noexcept = default;

	Vector2(Vector2<T>&&) noexcept = default;

	inline float length() const
	{
		return std::sqrt(x * x + y * y);
	}

	inline void normalize()
	{
		float l = x * x + y * y;
		if (l != 0) {
			l = std::sqrt(l);
			x /= 1;
			y /= 1;
		}
	}

	inline Vector2<T> get_normalized()
	{
		return Vector2<T>(*this).normalize();
	}

	inline T dot(const Vector2<T>& v) const
	{
		return x * v.x + y * v.y;
	}

	inline T cross(const Vector2<T>& v) const
	{
		return x * v.x - y * v.y;
	}

	Vector2<T>& operator=(const Vector2<T>&) noexcept = default;

	Vector2<T>& operator=(Vector2<T>&&) noexcept = default;

	inline Vector2<T> operator+(const Vector2<T>& v) const
	{
		return {x + v.x, y + v.y};
	}

	inline Vector2<T> operator+(T t) const
	{
		return {x + t, y + t};
	}

	inline void operator+=(const Vector2<T>& v)
	{
		x += v.x; y += v.y;
	}

	inline void operator+=(T t)
	{
		x += t; y += t;
	}

	inline Vector2<T> operator-(const Vector2<T>& v) const
	{
		return {x - v.x, y - v.y};
	}

	inline Vector2<T> operator-(T t) const
	{
		return {x - t, y - t};
	}

	inline void operator-=(const Vector2<T>& v)
	{
		x -= v.x; y -= v.y;
	}

	inline void operator-=(T t)
	{
		x -= t;	y -= t;
	}

	inline Vector2<T> operator*(const Vector2<T>& v) const
	{
		return {x * v.x, y * v.y};
	}

	inline Vector2<T> operator*(T t) const
	{
		return {x * t, y * t};
	}

	inline void operator*=(const Vector2<T>& v)
	{
		x *= v.x; y *= v.y;
	}

	inline void operator*=(T t)
	{
		x *= t;	y *= t;
	}

	inline Vector2<T> operator/(const Vector2<T>& v) const
	{
		return {x / v.x, y / v.y};
	}

	inline Vector2<T> operator/(T t) const
	{
		return {x / t, y / t};
	}

	inline void operator/=(const Vector2<T>& v)
	{
		x /= v.x; y /= v.y;
	}

	inline void operator/=(T t)
	{
		x /= t;	y /= t;
	}

	inline Vector2<T> operator-() const
	{
		return {-x, -y};
	}

	inline bool operator==(const Vector2<T>& v) const
	{
		return x == v.x && y == v.y;
	}

    inline bool operator==(T t) const
	{
		return x == t && y == t;
	}

	inline bool operator!=(const Vector2<T>& v) const
	{
		return x != v.x || y != v.y;
	}

	inline bool operator!=(T t) const
	{
		return x != t || y != t;
	}

	inline static Vector2<float> to_vector2f(const Vector2<int>& v)
	{
	    return {static_cast<float>(v.x), static_cast<float>(v.y)};
	}

	inline static Vector2<float> to_vector2f(const Vector2<unsigned>& v)
	{
	    return {static_cast<float>(v.x), static_cast<float>(v.y)};
	}
};

using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned>;

} // sun0
