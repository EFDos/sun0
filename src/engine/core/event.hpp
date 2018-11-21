/*************************************************************************/
/*  event.hpp                                                            */
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

#include "input/keyboard.hpp"

namespace sun {

enum class event_type
{
    undefined,
    closed,
    lost_focus, // unimplemented
    gained_focus, // unimplemented
    key_pressed,
    key_released,
    text_entered,
    mouse_wheel_scrolled,
    mouse_button_pressed,
    mouse_button_released,
    mouse_moved,
    mouse_entered, // unimplemented
    mouse_left, // unimplemented
    joystick_button_pressed,
    joystick_button_released,
    joystick_hat_moved,
    joystick_axis_moved,
    joystick_connected,
    joystick_disconnected,
    touch_began,
    touch_moved,
    touch_ended
};

class event
{
public:

    struct key_event
    {
        keyboard::key    code;
        bool            alt;
        bool            control;
        bool            shift;
        bool            system;
    };

    struct text_input_event
    {
        char text[32];
        uint8 text_size;
    };

    struct mouse_move_event
    {
        int32   x;
        int32   y;
    };

    struct mouse_button_event
    {
        mouse::button   button;
        int32   x;
        int32   y;
    };

    struct mouse_wheel_event
    {
        float   delta;
        int32   x;
        int32   y;
    };

    struct joystick_connect_event
    {
        uint8   id;
    };

    struct joystick_axis_event
    {
        uint8   id;
        uint8   axis;
        float   position;
    };

    struct joystick_button_event
    {
        uint8   id;
        uint8   button;
    };

    struct joystick_hat_event
    {
        uint8   id;
        uint8   hat;
        uint8   value;
    };

    struct touch_event
    {
        uint8   finger;
        int32     x;
        int32     y;
    };

    event_type  type = event_type::undefined;

    union
    {
        key_event               key;
        text_input_event        text_input;
        mouse_move_event        mouse_move;
        mouse_button_event      mouse_button;
        mouse_wheel_event       mouse_wheel_scroll;
        joystick_axis_event     joystick_axis;
        joystick_hat_event      joystick_hat;
        joystick_button_event   joystick_button;
        joystick_connect_event  joystick_connect;
        touch_event             touch;
    };

};

}
