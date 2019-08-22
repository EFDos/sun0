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

#include "common/config.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "input/joystick.hpp"

namespace sun {

enum class EventType : uint8
{
    Undefined,
    Closed,
    LostFocus, // unimplemented
    GainedFocus, // unimplemented
    KeyPressed,
    KeyReleased,
    TextEntered,
    MouseWheelScrolled,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseEntered, // unimplemented
    MouseLeft, // unimplemented
    JoystickButtonPressed,
    JoystickButtonReleased,
    JoystickHatMoved,
    JoystickAxisMoved,
    JoystickConnected,
    JoystickDisconnected,
    TouchBegan,
    TouchMoved,
    TouchEnded
};

class SUN_API Event
{
public:

    struct KeyEvent
    {
        keyboard::Key    code;
        bool            alt;
        bool            control;
        bool            shift;
        bool            system;
    };

    struct TextInputEvent
    {
        char text[32];
        uint8 size;
    };

    struct MouseMoveEvent
    {
        int32   x;
        int32   y;
        int32   x_rel;
        int32   y_rel;
    };

    struct MouseButtonEvent
    {
        mouse::Button   button;
        int32   x;
        int32   y;
    };

    struct MouseWheelEvent
    {
        float   delta;
        int32   x;
        int32   y;
    };

    struct JoystickConnectEvent
    {
        uint8   id;
    };

    struct JoystickAxisEvent
    {
        uint8   id;
        uint8   axis;
        float   position;
    };

    struct JoystickButtonEvent
    {
        uint8   id;
        uint8   button;
    };

    struct JoystickHatEvent
    {
        uint8   id;
        uint8   hat;
        uint8   value;
    };

    struct TouchEvent
    {
        uint8   finger;
        int32     x;
        int32     y;
    };

    EventType  type = EventType::Undefined;

    union
    {
        KeyEvent                key;
        TextInputEvent          text_input;
        MouseMoveEvent          mouse_move;
        MouseButtonEvent        mouse_button;
        MouseWheelEvent         mouse_wheel;
        JoystickAxisEvent       joystick_axis;
        JoystickHatEvent        joystick_hat;
        JoystickButtonEvent     joystick_button;
        JoystickConnectEvent    joystick_connect;
        TouchEvent              touch;
    };

};

} // sun
