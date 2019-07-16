/*************************************************************************/
/*  bar.cpp                                                              */
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
#include "bar.hpp"
#include "gui_system.hpp"
#include "graphics/renderer.hpp"

namespace sun {

Bar::Bar(Context& context, Orientation orientation)
:   Container(context),
    orientation_(orientation)
{
}

void Bar::draw(Renderer* renderer) const
{
    auto rectf = Recti::to_rectf(bounds_);
    renderer->draw_rect(rectf, gui_->get_default_theme().accent_color);
    Container::draw(renderer);
}

void Bar::handle_events(const Event& event)
{
    Container::handle_events(event);
}

void Bar::on_parent_set_()
{
    bounds_ = dynamic_cast<Container*>(parent_)->request_bounds({
        4, 8,
        parent_->get_bounding_rect().w - 8, 32
    });
}

Recti Bar::request_bounds(Recti&& bounds)
{
    return {0, 0, 0, 0};
}

}
