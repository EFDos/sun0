/*************************************************************************/
/*  math.hpp                                                             */
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

namespace sun {
namespace math {

constexpr float pi = 3.14159265358979323846f;

template<class T>
constexpr inline T& crop(T& val, T crop_up, T crop_down)
{
    if(val > crop_up)
        val = crop_up;
    else if(val < crop_down)
        val = crop_down;

    return val;
}

constexpr inline float rad_to_deg(float angle)
{
    return (angle * 180) / pi;
}

constexpr inline float deg_to_rad(float angle)
{
    return pi * (180 / angle);
}

}
}
