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
#include "graphics/renderer.hpp"

namespace sun {

Frame::Frame(Context& context, const Vector2i& size)
:   Widget(context)
{
    bounds_.set_size(size);
}

void Frame::draw(Renderer* renderer) const
{
    renderer->set_draw_mode(Renderer::DrawMode::Triangles);
    renderer->draw_rect(Recti::to_rectf(bounds_), Color::WHITE);
}

}