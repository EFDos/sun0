/*************************************************************************/
/*  widget.hpp                                                           */
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

#include "common/object.hpp"
#include "common/types.hpp"

namespace sun {

class Renderer;
class Event;
class GUISystem;

class SUN_API Widget : public Object
{
public:

    Widget(Context&);

    //virtual ~Widget();

    virtual void draw(Renderer*) const = 0;

    virtual void handle_events(const Event&) = 0;

    inline void set_parent(Widget* parent) {
        parent_ = parent;
        on_parent_set_();
    }

    inline Vector2i get_position() const {
        return bounds_.get_position();
    }

    inline Vector2i get_size() const {
        return bounds_.get_size();
    }

    inline const Recti& get_bounding_rect() const {
        return bounds_;
    }

    inline void set_gui_system(GUISystem* gui) {
        gui_ = gui;
    }

protected:

    virtual void on_parent_set_();

    GUISystem* gui_;
    Widget* parent_;
    Recti   bounds_;
};

}
