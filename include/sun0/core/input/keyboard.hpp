/*************************************************************************/
/*  keyboard.hpp                                                         */
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
#include <SDL2/SDL_keycode.h>

namespace sun {
namespace keyboard {

enum class Key
{
    Undefined = -1,
    A     = SDLK_a,
    B     = SDLK_b,
    C     = SDLK_c,
    D     = SDLK_d,
    E     = SDLK_e,
    F     = SDLK_f,
    G     = SDLK_g,
    H     = SDLK_h,
    I     = SDLK_i,
    J     = SDLK_j,
    K     = SDLK_k,
    L     = SDLK_l,
    M     = SDLK_m,
    N     = SDLK_n,
    O     = SDLK_o,
    P     = SDLK_p,
    Q     = SDLK_q,
    R     = SDLK_r,
    S     = SDLK_s,
    T     = SDLK_t,
    U     = SDLK_u,
    V     = SDLK_v,
    W     = SDLK_w,
    X     = SDLK_x,
    Y     = SDLK_y,
    Z     = SDLK_z,
    Num0,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    Escape          = SDLK_ESCAPE,
    LeftControl     = SDLK_LCTRL,
    LeftShift       = SDLK_LSHIFT,
    LeftAlt         = SDLK_LALT,
    LeftSystem,//   = SDLK_LSUPER,
    RightControl    = SDLK_RCTRL,
    RightShift      = SDLK_RSHIFT,
    RightAlt        = SDLK_RALT,
    RightSystem,//    = SDLK_RSUPER,
    Menu,
    LeftBracket,
    RightBracket,
    Semicolon,
    Comma,
    Period,
    Quote,
    Slash,
    Backslash,
    Tilde,
    Equals      = SDLK_EQUALS,
    Dash        = SDLK_MINUS,
    Space       = SDLK_SPACE,
    Return      = SDLK_RETURN,
    Backspace   = SDLK_BACKSPACE,
    Tab         = SDLK_TAB,
    PageUp,
    PageDown,
    End,
    Home        = SDLK_HOME,
    Insert,
    Delete,
    Add         = SDLK_EQUALS,
    Subtract    = SDLK_MINUS,
    Multiply,
    Divide,
    Left        = SDLK_LEFT,
    Right       = SDLK_RIGHT,
    Up          = SDLK_UP,
    Down        = SDLK_DOWN,
    NumPad0,
    NumPad1,
    NumPad2,
    NumPad3,
    NumPad4,
    NumPad5,
    NumPad6,
    NumPad7,
    NumPad8,
    NumPad9,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    Pause,
};

bool is_key_pressed(Key k);

} // keyboard
} // sun
