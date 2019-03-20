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

enum class key
{
    UNDEF = -1,
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
    NUM0,
    NUM1,
    NUM2,
    NUM3,
    NUM4,
    NUM5,
    NUM6,
    NUM7,
    NUM8,
    NUM9,
    ESCAPE      = SDLK_ESCAPE,
    CONTROL_L   = SDLK_LCTRL,
    SHIFT_L     = SDLK_LSHIFT,
    ALT_L       = SDLK_LALT,
    SYSTEM_L,//    = SDLK_LSUPER,
    CONTROL_R   = SDLK_RCTRL,
    SHIFT_R     = SDLK_RSHIFT,
    ALT_R       = SDLK_RALT,
    SYSTEM_R,//    = SDLK_RSUPER,
    MENU,
    BRACKET_L,
    BRACKET_R,
    SEMICOLON,
    COMMA,
    PERIOD,
    QUOTE,
    SLASH,
    BACKSLASH,
    TILDE,
    EQUAL       = SDLK_EQUALS,
    DASH        = SDLK_MINUS,
    SPACE       = SDLK_SPACE,
    RETURN      = SDLK_RETURN,
    BACKSPACE   = SDLK_BACKSPACE,
    TAB         = SDLK_TAB,
    PAGEUP,
    PAGEDOWN,
    END,
    HOME        = SDLK_HOME,
    INSERT,
    DELETE,
    ADD         = SDLK_EQUALS,
    SUBTRACT    = SDLK_MINUS,
    MULTIPLY,
    DIVIDE,
    LEFT        = SDLK_LEFT,
    RIGHT       = SDLK_RIGHT,
    UP          = SDLK_UP,
    DOWN        = SDLK_DOWN,
    NUMPAD0,
    NUMPAD1,
    NUMPAD2,
    NUMPAD3,
    NUMPAD4,
    NUMPAD5,
    NUMPAD6,
    NUMPAD7,
    NUMPAD8,
    NUMPAD9,
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
    PAUSE,
};

bool is_key_pressed(key k);

} // keyboard
} // sun
