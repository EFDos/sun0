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
#include "shape2D.hpp"

#include "common/shapes/primitive_shape.hpp"
#include "core/context.hpp"
#include "renderer.hpp"

#include <vector>

namespace sun {

shape2D::shape2D(context& p_context)
:   drawable(p_context),
    vertices_(nullptr),
    indices_(nullptr)
{
    auto r = context_.get_system<renderer>();
    vertices_ = r->create_vertex_buffer(sizeof(float) * 6, 0);
    indices_ = r->create_index_buffer(0);
}

shape2D::~shape2D()
{
    delete vertices_;
    delete indices_;
}

void shape2D::set_shape(const shapes::primitive_shape& shape)
{
    vertices_->clear();
    vertices_->resize(shape.get_point_count());
    std::vector<float> vertex_data(shape.get_point_count() * 6);
    bool quad = false;
    auto col = to_colorf(color_);

    for (size_t i = 0 ; i < shape.get_point_count() ; ++i)
    {

        vertex_data[i * 6 + 0] = shape.get_point(i).x;
        vertex_data[i * 6 + 1] = shape.get_point(i).y;
        vertex_data[i * 6 + 2] = col.r;
        vertex_data[i * 6 + 3] = col.g;
        vertex_data[i * 6 + 4] = col.b;
        vertex_data[i * 6 + 5] = col.a;
    }

    switch(shape.get_point_count())
    {
        case 1:
            draw_mode_ = renderer::draw_mode::points;
            break;
        case 2:
            draw_mode_ = renderer::draw_mode::lines;
            break;
        case 3:
            draw_mode_ = renderer::draw_mode::triangles;
            break;
        case 4:
            draw_mode_ = renderer::draw_mode::triangles;
            quad = true;
            break;
        default:
            draw_mode_ = renderer::draw_mode::triangle_fan;
    }

    if (quad) {
        std::vector<uint32> indices = {0, 1, 3, 1, 2, 3};
        indices_->resize(6);
        indices_->fill_data(0, 6, indices.data());
    } else {
        indices_->clear();
    }

    vertices_->fill_data(0, shape.get_point_count(), vertex_data.data());
}

void shape2D::draw(renderer* r) const
{
    r->set_draw_mode(draw_mode_);
    if (indices_->get_index_count() == 0) {
        r->draw(*vertices_);
    } else {
        r->draw_indexed(*vertices_, *indices_);
    }
    // Reset draw mode to default state
    if (draw_mode_ != renderer::draw_mode::triangles) {
        r->set_draw_mode(renderer::draw_mode::triangles);
    }
}

}
