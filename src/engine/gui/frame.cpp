/*************************************************************************/
/*  frame.hpp                                                            */
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
#include "frame.hpp"
#include "gui_system.hpp"
#include "graphics/renderer.hpp"
#include "core/event.hpp"

namespace sun {

Frame::Frame(Context& context, const Vector2i& size)
:   Container(context),
    grabbed_(false)
{
    bounds_.set_size(size);
}

void Frame::draw(Renderer* renderer) const
{
    auto rectf = Recti::to_rectf(bounds_);
    auto border = rectf;
    border.x -= 4.f;
    border.y -= 4.f;
    border.w += 8.f;
    border.h += 8.f;
    renderer->draw_rect(border, gui_->get_default_theme().accent_color);
    renderer->draw_rect(rectf, gui_->get_default_theme().main_color);

    Container::draw(renderer);
}

void Frame::handle_events(const Event& event)
{
    if (event.type == EventType::MouseButtonPressed)
    {
        if (bounds_.contains({
            event.mouse_move.x,
            event.mouse_move.y}))
        {
            if (event.mouse_button.button == mouse::Button::Middle) {
                grabbed_ = true;
            }
        }
    }

    if (event.type == EventType::MouseButtonReleased)
    {
        if (event.mouse_button.button == mouse::Button::Middle) {
            grabbed_ = false;
        }
    }

    if (grabbed_ && event.type == EventType::MouseMoved) {
        bounds_.x += event.mouse_move.x_rel;
        bounds_.y += event.mouse_move.y_rel;
    }
    Container::handle_events(event);
}

Recti Frame::request_bounds(Recti&& bounds) {
    if (bounds_.contains(bounds))
    {
        return bounds;
    }

    return {0, 0, 0, 0};
}

}
