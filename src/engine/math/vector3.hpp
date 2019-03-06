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
class vector3
{
public:

	T x, y, z;

	vector3(T p_x = 0, T p_y = 0, T p_z = 0) noexcept
	: x(p_x), y(p_y), z(p_z) {}

	vector3(const T* data) : x(data[0]), y(data[1]), z(data[2])
	{}

	vector3(const vector3<T>&) noexcept = default;

	vector3(vector3<T>&&) noexcept = default;

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

	inline vector3<T> get_normalized()
	{
		return vector3<T>(*this).normalize();
	}

	inline T dot(const vector3<T>& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline T cross(const vector3<T>& v) const
	{
		return x * v.x - y * v.y - z * v.z;
	}

	vector3<T>& operator=(const vector3<T>&) noexcept = default;

	vector3<T>& operator=(vector3<T>&&) noexcept = default;

	inline vector3<T> operator+(const vector3<T>& v) const
	{
		return {x + v.x, y + v.y, z + v.z};
	}

	inline vector3<T> operator+(T t) const
	{
		return {x + t, y + t, z + t};
	}

	inline void operator+=(const vector3<T>& v)
	{
		x += v.x; y += v.y; z += v.z;
	}

	inline void operator+=(T t)
	{
		x += t; y += t; z += t;
	}

	inline vector3<T> operator-(const vector3<T>& v) const
	{
		return {x - v.x, y - v.y, z - v.z};
	}

	inline vector3<T> operator-(T t) const
	{
		return {x - t, y - t, z - t};
	}

	inline void operator-=(const vector3<T>& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
	}

	inline void operator-=(T t)
	{
		x -= t;	y -= t; z -= t;
	}

	inline vector3<T> operator*(const vector3<T>& v) const
	{
		return {x * v.x, y * v.y, z * v.z};
	}

	inline vector3<T> operator*(T t) const
	{
		return {x * t, y * t, z * t};
	}

	inline void operator*=(const vector3<T>& v)
	{
		x *= v.x; y *= v.y; z *= v.z;
	}

	inline void operator*=(T t)
	{
		x *= t;	y *= t; z *= t;
	}

	inline vector3<T> operator/(const vector3<T>& v) const
	{
		return {x / v.x, y / v.y, z / v.z};
	}

	inline vector3<T> operator/(T t) const
	{
		return {x / t, y / t, z / t};
	}

	inline void operator/=(const vector3<T>& v)
	{
		x /= v.x; y /= v.y; z /= v.z;
	}

	inline void operator/=(T t)
	{
		x /= t;	y /= t; z /= t;
	}

	inline vector3<T> operator-() const
	{
		return {-x, -y, -z};
	}

	inline bool operator==(const vector3<T>& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	inline bool operator!=(const vector3<T>& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}
};

using vector3f = vector3<float>;
using vector3d = vector3<double>;
using vector3i = vector3<int>;
using vector3u = vector3<unsigned>;

} // sun0
