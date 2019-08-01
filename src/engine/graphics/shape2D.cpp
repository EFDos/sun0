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

#include "common/shapes/shape.hpp"
#include "common/shapes/rectangle.hpp"
#include "common/shapes/circle.hpp"
#include "common/shapes/convex.hpp"
#include "core/context.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"

#include <vector>
#include <algorithm>

namespace sun {

Shape2D::Shape2D(Context& context)
:   Drawable(context),
    shape_(nullptr),
    color_(sun::Color::WHITE),
    vertices_(nullptr),
    indices_(nullptr)
{
    auto r = context_.get_system<Renderer>();
    vertices_ = r->create_vertex_buffer(sizeof(float) * 6, 0);
    indices_ = r->create_index_buffer(0);
}

Shape2D::~Shape2D()
{
    delete vertices_;
    delete indices_;
    if (shape_ == nullptr) {
        delete shape_;
    }
}

void Shape2D::set_shape(const shapes::Shape& shape)
{
    switch(shape.get_type())
    {
        case shapes::ShapeType::Rectangle:
            shape_ = new shapes::Rectangle(
                static_cast<const shapes::Rectangle&>(shape)
            );
            break;
        case shapes::ShapeType::Circle:
            shape_ = new shapes::Circle(
                static_cast<const shapes::Circle&>(shape)
            );
            break;
        case shapes::ShapeType::Convex:
            shape_ = new shapes::Convex(
                static_cast<const shapes::Convex&>(shape)
            );
            break;
    }
    update_geometry_();
}

void Shape2D::update_geometry_()
{
    if (shape_ == nullptr) {
        return;
    }
    vertices_->clear();
    vertices_->resize(shape_->get_point_count());
    std::vector<float> vertex_data(shape_->get_point_count() * 6);
    bool quad = false;
    int far_x = 0, far_y = 0;
    auto col = Color::to_colorf(color_);

    for (size_t i = 0 ; i < shape_->get_point_count() ; ++i)
    {
        const auto& point = shape_->get_point(i);
        far_x = std::max(far_x, static_cast<int>(point.x));
        far_y = std::max(far_y, static_cast<int>(point.y));
        vertex_data[i * 6 + 0] = point.x;
        vertex_data[i * 6 + 1] = point.y;
        vertex_data[i * 6 + 2] = col.r;
        vertex_data[i * 6 + 3] = col.g;
        vertex_data[i * 6 + 4] = col.b;
        vertex_data[i * 6 + 5] = col.a;
    }

    bounding_rect_ = { 0, 0, far_x, far_y };

    switch(shape_->get_type())
    {
        case shapes::ShapeType::Rectangle:
            draw_mode_ = Renderer::DrawMode::Triangles;
            quad = true;
            break;
        case shapes::ShapeType::Circle:
            draw_mode_ = Renderer::DrawMode::TriangleFan;
            break;
        case shapes::ShapeType::Convex:
            draw_mode_ = Renderer::DrawMode::Triangles;
            break;
    }

    if (quad) {
        std::vector<uint32> indices = {0, 1, 3, 1, 2, 3};
        indices_->resize(6);
        indices_->fill_data(0, 6, indices.data());
    } else {
        indices_->clear();
    }

    vertices_->fill_data(0, shape_->get_point_count(), vertex_data.data());
}

void Shape2D::draw(Renderer* renderer) const
{
    if (owning_entity_ != nullptr) {
        renderer->set_model_transform(owning_entity_->get_global_transform());
    } else if (transform_ != nullptr) {
        renderer->set_model_transform(*transform_);
    }
    renderer->set_draw_mode(draw_mode_);
    if (indices_->get_index_count() == 0) {
        renderer->draw(*vertices_);
    } else {
        renderer->draw_indexed(*vertices_, *indices_);
    }
    // Reset draw mode to default state
    if (draw_mode_ != Renderer::DrawMode::Triangles) {
        renderer->set_draw_mode(Renderer::DrawMode::Triangles);
    }
}

}
