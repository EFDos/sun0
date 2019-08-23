/*************************************************************************/
/*  shape2D.hpp                                                          */
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

#include "common/types.hpp"

#include "renderer.hpp"
#include "drawable.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace sun {

namespace shapes {
class Shape;
}

class SUN_API Shape2D final : public Drawable
{
public:

    SUN_COMPONENT_TYPE(Shape2D)

    Shape2D(Context& context);

    ~Shape2D();

    void init() override;

    void draw(Renderer* renderer) override;

    void set_shape(const shapes::Shape& p_shape);

    inline void set_color(const Color& c) {
        color_ = c;
        dirty_ = true;
    }

private:

    void update_geometry_() override;

    //TODO Maybe change Shape* to union?
    shapes::Shape*              shape_;
    Color                       color_;
    Renderer::DrawMode          draw_mode_;
    VertexBuffer*               vertices_;
    IndexBuffer*                indices_;
};

}
