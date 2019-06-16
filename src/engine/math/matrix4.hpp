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

class Matrix4
{
public:

    Matrix4() noexcept;

    Matrix4(float e00, float e01, float e02,
            float e10, float e11, float e12,
            float e20, float e21, float e22) noexcept;

    Matrix4(const Matrix4&) noexcept = default;

    Matrix4(Matrix4&&) noexcept = default;

    void set_identity();

    void set_transformation(float x, float y, float angle,
            float sx, float sy, float ox,
            float oy, float kx, float ky);

    void set_translation(const Vector2f& v);

    void set_translation(float x, float y);

    void set_scale(const Vector2f& v);

    void set_scale(float x, float y);

    void set_rotation(float angle);

    Matrix4 combine(const Matrix4& m) const;

    Matrix4& translate(const Vector2f& v);

    Matrix4& translate(float x, float y);

    Matrix4& scale(const Vector2f& v);

    Matrix4& scale(float x, float y);

    Matrix4& rotate(float angle);

    Matrix4 get_inverse() const;

    const float* get_data() const;

    Matrix4& operator=(const Matrix4&) noexcept;

    Matrix4& operator=(Matrix4&&) = default;

    Matrix4 operator*(const Matrix4& other) const;

    void operator*=(const Matrix4& other);

    static Matrix4 orthogonal(float left, float right, float bottom, float top);

    static Matrix4 orthogonal(const rect<float>& r);

private:

    float m_[16];
};

} // sun
