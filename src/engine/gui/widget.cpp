/*************************************************************************/
/*  widget.cpp                                                           */
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
#include "widget.hpp"
#include "gui_system.hpp"
#include "core/context.hpp"
#include "core/event.hpp"

namespace sun {

Widget::Widget(Context& context)
:   Object(context),
    gui_(nullptr),
    parent_(nullptr),
    grabbed_(false)
{}

void Widget::handle_events(const Event& event)
{
    if (event.type == EventType::MouseButtonPressed) {
        if (event.mouse_button_event.button == mouse::Button::Left) {
            grabbed_ = true;
        }
    }
    if (event.type == EventType::MouseButtonReleased) {
        if (event.mouse_button_event.button == mouse::Button::Left) {
            grabbed_ = false;
        }
    }
}

void Widget::on_parent_set_()
{}

}
