/*************************************************************************/
/*  vector3.hpp                                                          */
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
class Vector3
{
public:

	T x, y, z;

	Vector3(T p_x = 0, T p_y = 0, T p_z = 0) noexcept
	: x(p_x), y(p_y), z(p_z) {}

	Vector3(const T* data) : x(data[0]), y(data[1]), z(data[2])
	{}

	Vector3(const Vector3<T>&) noexcept = default;

	Vector3(Vector3<T>&&) noexcept = default;

	inline float length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	inline void normalize()
	{
		float l = x * x + y * y + z * z;
		if (l != 0) {
			l = std::sqrt(l);
			x /= 1;
			y /= 1;
			z /= 1;
		}
	}

	inline Vector3<T> get_normalized()
	{
		return Vector3<T>(*this).normalize();
	}

	inline T dot(const Vector3<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline T cross(const Vector3<T>& v) const
	{
		return x * v.x - y * v.y - z * v.z;
	}

	Vector3<T>& operator=(const Vector3<T>&) noexcept = default;

	Vector3<T>& operator=(Vector3<T>&&) noexcept = default;

	inline Vector3<T> operator+(const Vector3<T>& v) const
	{
		return {x + v.x, y + v.y, z + v.z};
	}

	inline Vector3<T> operator+(T t) const
	{
		return {x + t, y + t, z + t};
	}

	inline void operator+=(const Vector3<T>& v)
	{
		x += v.x; y += v.y; z += v.z;
	}

	inline void operator+=(T t)
	{
		x += t; y += t; z += t;
	}

	inline Vector3<T> operator-(const Vector3<T>& v) const
	{
		return {x - v.x, y - v.y, z - v.z};
	}

	inline Vector3<T> operator-(T t) const
	{
		return {x - t, y - t, z - t};
	}

	inline void operator-=(const Vector3<T>& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	inline void operator-=(T t)
	{
		x -= t;	y -= t; z -= t;
	}

	inline Vector3<T> operator*(const Vector3<T>& v) const
	{
		return {x * v.x, y * v.y, z * v.z};
	}

	inline Vector3<T> operator*(T t) const
	{
		return {x * t, y * t, z * t};
	}

	inline void operator*=(const Vector3<T>& v)
	{
		x *= v.x; y *= v.y; z *= v.z;
	}

	inline void operator*=(T t)
	{
		x *= t;	y *= t; z *= t;
	}

	inline Vector3<T> operator/(const Vector3<T>& v) const
	{
		return {x / v.x, y / v.y, z / v.z};
	}

	inline Vector3<T> operator/(T t) const
	{
		return {x / t, y / t, z / t};
	}

	inline void operator/=(const Vector3<T>& v)
	{
		x /= v.x; y /= v.y; z /= v.z;
	}

	inline void operator/=(T t)
	{
		x /= t;	y /= t; z /= t;
	}

	inline Vector3<T> operator-() const
	{
		return {-x, -y, -z};
	}

	inline bool operator==(const Vector3<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	inline bool operator!=(const Vector3<T>& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3i = Vector3<int>;
using Vector3u = Vector3<unsigned>;

} // sun0
