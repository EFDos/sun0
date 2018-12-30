/*************************************************************************/
/*  matrix4.hpp                                                          */
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
#include "rect.hpp"

namespace sun {

class matrix4
{
public:

    matrix4() noexcept;

    matrix4(float e00, float e01, float e02,
            float e10, float e11, float e12,
            float e20, float e21, float e22) noexcept;

    matrix4(const matrix4&) noexcept = default;

    matrix4(matrix4&&) noexcept = default;

    void set_identity();

    void set_transformation(float x, float y, float angle,
            float sx, float sy, float ox,
            float oy, float kx, float ky);

    void set_translation(const vector2f& v);

    void set_translation(float x, float y);

    void set_scale(const vector2f& v);

    void set_scale(float x, float y);

    void set_rotation(float angle);

    matrix4 combine(const matrix4& m) const;

    matrix4& translate(const vector2f& v);

    matrix4& translate(float x, float y);

    matrix4& scale(const vector2f& v);

    matrix4& scale(float x, float y);

    matrix4& rotate(float angle);

    matrix4 get_inverse() const;

    const float* get_data() const;

    matrix4& operator=(const matrix4&) noexcept;

    matrix4& operator=(matrix4&&) = default;

    matrix4 operator*(const matrix4& other) const;

    void operator*=(const matrix4& other);

    static matrix4 orthogonal(float left, float right, float bottom, float top);

    static matrix4 orthogonal(const rect<float>& r);

private:

    float m_[16];
};

} // sun
