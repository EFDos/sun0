/*************************************************************************/
/*  container.hpp                                                        */
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

#include "widget.hpp"

#include <vector>

namespace sun {

class SUN_API Container : public Widget
{
public:

    Container(Context& context) : Widget(context) {}

    virtual ~Container() {
        for (auto child : children_) {
            delete child;
        }
    }

    inline virtual void draw(Renderer* renderer) const override {
        for (auto child : children_) {
            child->draw(renderer);
        }
    }

    inline virtual void handle_events(const Event& event) override {
        for (auto child : children_) {
            child->handle_events(event);
        }
    }

    inline virtual void add_child(Widget* child) {
        children_.push_back(child);
        child->set_parent(this);
        child->set_gui_system(gui_);
    }

    virtual Recti request_bounds(Recti&& bounds) = 0;

protected:

    std::vector<Widget*>    children_;
};

}
