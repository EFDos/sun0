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
BaseColor<uint8> BaseColor<uint8>::RED = BaseColor<uint8>(255);

template<>
BaseColor<uint8> BaseColor<uint8>::GREEN = BaseColor<uint8>(0, 255);

template<>
BaseColor<uint8> BaseColor<uint8>::BLUE = BaseColor<uint8>(0, 0, 255);

template<>
BaseColor<uint8> BaseColor<uint8>::WHITE = BaseColor<uint8>(255, 255, 255);

template<>
BaseColor<uint8> BaseColor<uint8>::BLACK = BaseColor<uint8>();

template<>
BaseColor<uint8> BaseColor<uint8>::YELLOW = BaseColor<uint8>(255, 255, 0);

template<>
BaseColor<uint8> BaseColor<uint8>::MAGENTA = BaseColor<uint8>(255, 0, 255);

template<>
BaseColor<uint8> BaseColor<uint8>::SUN = BaseColor<uint8>(255, 255, 200);

template<>
BaseColor<uint8> BaseColor<uint8>::LIGHT_GREY = BaseColor<uint8>(100, 100, 100);

template<>
BaseColor<uint8> BaseColor<uint8>::DARK_GREY = BaseColor<uint8>(30, 30, 30);

}
