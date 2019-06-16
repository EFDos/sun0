/*************************************************************************/
/*  joystick.hpp                                                         */
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

#include "common/int.hpp"
#include <SDL2/SDL_joystick.h>

namespace sun {
namespace joystick {

    enum class Hat : uint8
    {
        Center      = SDL_HAT_CENTERED,
        Up          = SDL_HAT_UP,
        Right       = SDL_HAT_RIGHT,
        Left        = SDL_HAT_LEFT,
        Down        = SDL_HAT_DOWN,
        RightUp    = SDL_HAT_RIGHTUP,
        RightDown  = SDL_HAT_RIGHTDOWN,
        LeftUp     = SDL_HAT_LEFTUP,
        LeftDown   = SDL_HAT_LEFTDOWN
    };

} // joystick
} // sun
