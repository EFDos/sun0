/*************************************************************************/
/*  matrix4.cpp                                                          */
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
#include "matrix4.hpp"
#include <cstring>

namespace sun
{

Matrix4 Matrix4::orthogonal(float l, float r, float b, float t)
{
    Matrix4 m;

	m.m_[0] = 2.0f / (r - l);
	m.m_[5] = 2.0f / (t - b);
	m.m_[10] = -1.f;
	m.m_[12] = -(r + l) / (r - l);
	m.m_[13] = -(t + b) / (t - b);

	return m;
}

Matrix4 Matrix4::orthogonal(const rect<float>& r)
{
    Matrix4 m;

    m.m_[0] = 2.0f / (r.w - r.x);
    m.m_[5] = 2.0f / (r.y - r.h);
    m.m_[10] = -1.f;
    m.m_[12] = -(r.w + r.x) / (r.w - r.x);
    m.m_[13] = -(r.y + r.h) / (r.y - r.h);

    return m;
}

Matrix4::Matrix4() noexcept
{
    set_identity();
}

Matrix4::Matrix4(float x,  float y,  float angle,
        float sx, float sy, float ox,
        float oy, float kx, float ky) noexcept
{
    set_transformation(x,y,angle,sx,sy,ox,oy,kx,ky);
}

Matrix4 Matrix4::operator*(const Matrix4& m) const
{
    return combine(m);
}

void Matrix4::operator*=(const Matrix4& m)
{
    Matrix4 t = this->operator*(m);
    memcpy(m_, t.m_, sizeof(float)*16);
}

Matrix4& Matrix4::operator=(const Matrix4& m) noexcept
{
    memcpy(m_, m.m_, sizeof(float)*16);
    return *this;
}

Matrix4 Matrix4::combine(const Matrix4& m) const
{
    Matrix4 t;

    t.m_[0] = (m_[0]*m.m_[0]) + (m_[4]*m.m_[1]) + (m_[8]*m.m_[2]) + (m_[12]*m.m_[3]);
	t.m_[4] = (m_[0]*m.m_[4]) + (m_[4]*m.m_[5]) + (m_[8]*m.m_[6]) + (m_[12]*m.m_[7]);
	t.m_[8] = (m_[0]*m.m_[8]) + (m_[4]*m.m_[9]) + (m_[8]*m.m_[10]) + (m_[12]*m.m_[11]);
	t.m_[12] = (m_[0]*m.m_[12]) + (m_[4]*m.m_[13]) + (m_[8]*m.m_[14]) + (m_[12]*m.m_[15]);

	t.m_[1] = (m_[1]*m.m_[0]) + (m_[5]*m.m_[1]) + (m_[9]*m.m_[2]) + (m_[13]*m.m_[3]);
	t.m_[5] = (m_[1]*m.m_[4]) + (m_[5]*m.m_[5]) + (m_[9]*m.m_[6]) + (m_[13]*m.m_[7]);
	t.m_[9] = (m_[1]*m.m_[8]) + (m_[5]*m.m_[9]) + (m_[9]*m.m_[10]) + (m_[13]*m.m_[11]);
	t.m_[13] = (m_[1]*m.m_[12]) + (m_[5]*m.m_[13]) + (m_[9]*m.m_[14]) + (m_[13]*m.m_[15]);

	t.m_[2] = (m_[2]*m.m_[0]) + (m_[6]*m.m_[1]) + (m_[10]*m.m_[2]) + (m_[14]*m.m_[3]);
	t.m_[6] = (m_[2]*m.m_[4]) + (m_[6]*m.m_[5]) + (m_[10]*m.m_[6]) + (m_[14]*m.m_[7]);
	t.m_[10] = (m_[2]*m.m_[8]) + (m_[6]*m.m_[9]) + (m_[10]*m.m_[10]) + (m_[14]*m.m_[11]);
	t.m_[14] = (m_[2]*m.m_[12]) + (m_[6]*m.m_[13]) + (m_[10]*m.m_[14]) + (m_[14]*m.m_[15]);

	t.m_[3] = (m_[3]*m.m_[0]) + (m_[7]*m.m_[1]) + (m_[11]*m.m_[2]) + (m_[15]*m.m_[3]);
	t.m_[7] = (m_[3]*m.m_[4]) + (m_[7]*m.m_[5]) + (m_[11]*m.m_[6]) + (m_[15]*m.m_[7]);
	t.m_[11] = (m_[3]*m.m_[8]) + (m_[7]*m.m_[9]) + (m_[11]*m.m_[10]) + (m_[15]*m.m_[11]);
	t.m_[15] = (m_[3]*m.m_[12]) + (m_[7]*m.m_[13]) + (m_[11]*m.m_[14]) + (m_[15]*m.m_[15]);

    return t;
}

void Matrix4::set_identity()
{
    for(auto &e : m_)
        e = 0.f;

    m_[0] = m_[5] = m_[10] = m_[15] = 1.f;
}

void Matrix4::set_translation(float x, float y)
{
    m_[12] = x;
    m_[13] = y;
}

void Matrix4::set_translation(const Vector2f& v)
{
    m_[12] = v.x;
    m_[13] = v.y;
}

void Matrix4::set_rotation(float r)
{
    r = r * 3.14f / 180.f;
    float c = std::cos(r), s = std::sin(r);
    m_[0] = c;
    m_[4] = -s;
    m_[1] = s;
    m_[5] = c;
}

void Matrix4::set_scale(float x, float y)
{
    m_[0] = x;
    m_[5] = y;
}

void Matrix4::set_scale(const Vector2f& v)
{
    m_[0] = v.x;
    m_[5] = v.y;
}

void Matrix4::set_transformation(float e00, float e01, float e02,
                                 float e10, float e11, float e12,
                                 float e20, float e21, float e22)
{
    m_[0] = e00; m_[4] = e01; m_[8]  = 0.f; m_[12] = e02;
    m_[1] = e10; m_[5] = e11; m_[9]  = 0.f; m_[13] = e12;
    m_[2] = 0.f; m_[6] = 0.f; m_[10] = 1.f; m_[14] = 0.f;
    m_[3] = e20; m_[7] = e21; m_[11] = 0.f; m_[15] = e22;
}

Matrix4 Matrix4::get_inverse() const
{
    float det = m_[0] * (m_[15] * m_[5] - m_[7] * m_[13]) -
            m_[1] * (m_[15] * m_[4] - m_[7] * m_[12]) +
            m_[3] * (m_[13] * m_[4] - m_[5] * m_[12]);

    if(det != 0.f)
    {
        return Matrix4((m_[15] * m_[5] - m_[7] * m_[13]) / det,
                -(m_[15] * m_[4] - m_[7] * m_[12]) / det,
                 (m_[13] * m_[4] - m_[5] * m_[12]) / det,
                -(m_[15] * m_[1] - m_[3] * m_[13]) / det,
                 (m_[15] * m_[0] - m_[3] * m_[12]) / det,
                -(m_[13] * m_[0] - m_[1] * m_[12]) / det,
                 (m_[7]  * m_[1] - m_[3] * m_[5])  / det,
                -(m_[7]  * m_[0] - m_[3] * m_[4])  / det,
                 (m_[5]  * m_[0] - m_[1] * m_[4])  / det);
    }
    else{
        return Matrix4();
    }
}

const float* Matrix4::get_data() const
{
    return m_;
}

Matrix4& Matrix4::rotate(float a)
{
    Matrix4 t;
    t.set_rotation(a);
    *this *= t;
    return *this;
}

Matrix4& Matrix4::scale(const Vector2f& s)
{
    Matrix4 t;
    t.set_scale(s);
    *this *= t;
    return *this;
}

Matrix4& Matrix4::scale(float x, float y)
{
    Matrix4 t;
    t.set_scale(x,y);
    *this *= t;
    return *this;
}

Matrix4& Matrix4::translate(const Vector2f& v)
{
    Matrix4 t;
    t.set_translation(v);
    *this *= t;
    return *this;
}

Matrix4& Matrix4::translate(float x, float y)
{
    Matrix4 t;
    t.set_translation(x,y);
    *this *= t;
    return *this;
}

} // sun

