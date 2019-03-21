/*************************************************************************/
/*  color.cpp                                                            */
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
#include "color.hpp"

namespace sun {

template<>
base_color<uint8> base_color<uint8>::red = base_color<uint8>(255);

template<>
base_color<uint8> base_color<uint8>::green = base_color<uint8>(0, 255);

template<>
base_color<uint8> base_color<uint8>::blue = base_color<uint8>(0, 0, 255);

template<>
base_color<uint8> base_color<uint8>::white = base_color<uint8>(255, 255, 255);

template<>
base_color<uint8> base_color<uint8>::black = base_color<uint8>();

template<>
base_color<uint8> base_color<uint8>::yellow = base_color<uint8>(255, 255, 0);

template<>
base_color<uint8> base_color<uint8>::magenta = base_color<uint8>(255, 0, 255);

template<>
base_color<uint8> base_color<uint8>::sun = base_color<uint8>(255, 255, 200);

template<>
base_color<uint8> base_color<uint8>::light_grey = base_color<uint8>(100, 100, 100);

template<>
base_color<uint8> base_color<uint8>::dark_grey = base_color<uint8>(30, 30, 30);

}
