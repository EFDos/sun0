/*************************************************************************/
/*  renderer.cpp                                                         */
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
#include "renderer.hpp"
#include "core/logger.hpp"

#include "font.hpp"

#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "text.hpp"
#include "shape2D.hpp"

namespace sun {

renderer::renderer(context& c)
:   system(c),
    draw_mode_(draw_mode::triangles),
    primitive_vertices_(nullptr),
    primitive_indices_(nullptr),
    current_shader_(nullptr),
    current_texture_(nullptr)
{
}

bool renderer::init()
{

    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_warn("Graphics backend did not intialize buffers"
            " for primitives rendering");
    }

    sun_log_info("Graphics System ready.");
    return true;
}

void renderer::shutdown()
{
    for (auto drawable : drawables_) {
        delete drawable;
    }
    sun_log_info("Graphics System shutdown.");
}

void renderer::set_color(const color& col)
{
    clear_color_ = to_colorf(col);
}

void renderer::update()
{
    clear();
    for (auto s : drawables_) {
        draw(*s);
    }
}

void renderer::draw_rect(const rectf& rect, const color& c) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    auto fcolor = to_colorf(c);
    std::vector<float> vertices{
        rect.x, rect.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x + rect.w, rect.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x + rect.w, rect.y + rect.h,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x, rect.y + rect.h,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
    };

    draw_mode_ = draw_mode::triangles;

    uint32 indices[] = {0, 1, 3, 1, 2, 3};

    if (primitive_indices_->get_index_count() != 6) {
        primitive_indices_->resize(6);
    }
    primitive_indices_->fill_data(0, 6, indices);

    if (primitive_vertices_->get_vertex_count() < 4) {
        primitive_vertices_->resize(4);
    }
    primitive_vertices_->fill_data(0, 4, vertices.data());

    draw_indexed(*primitive_vertices_, *primitive_indices_);
}

void renderer::draw_circle(const vector2f& pos, float radius, int verts) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    /*auto fcolor = to_colorf(c);
    std::vector<float> vertices{
        rect.x, rect.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x + rect.w, rect.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x + rect.w, rect.y + rect.h,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        rect.x, rect.y + rect.h,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
    };

    draw_mode_ = draw_mode::triangles;

    uint32 indices[] = {0, 1, 3, 1, 2, 3};

    if (primitive_indices_->get_index_count() == 0) {
        primitive_indices_->resize(6);
    }
    primitive_indices_->fill_data(0, 6, indices);

    if (primitive_vertices_->get_vertex_count() < 4) {
        primitive_vertices_->resize(4);
    }
    primitive_vertices_->fill_data(0, 4, vertices.data());

    draw_indexed(*primitive_vertices_, *primitive_indices_);*/
}

void renderer::draw_line(const vector2f& begin, const vector2f& end, const color& c) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    auto fcolor = to_colorf(c);
    std::vector<float> vertices{
        begin.x, begin.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        end.x, end.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
    };

    draw_mode_ = draw_mode::lines;

    primitive_vertices_->resize(2);
    primitive_vertices_->fill_data(0, 2, vertices.data());

    draw(*primitive_vertices_);
}

void renderer::draw_polygon(uint vert_count,
                            const vector2f* verts,
                            const color& c) const
{
    if (vert_count < 3) {
        sun_log_error("Can't draw polygon with less than 3 vertices");
        return;
    }

    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }


    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    auto fcolor = to_colorf(c);
    std::vector<float> vertices(vert_count * 6);

    for (uint i = 0 ; i < vert_count ; ++i)
    {
        vertices[i * 6 + 0] = verts[i].x;
        vertices[i * 6 + 1] = verts[i].y;
        vertices[i * 6 + 2] = fcolor.r;
        vertices[i * 6 + 3] = fcolor.g;
        vertices[i * 6 + 4] = fcolor.b;
        vertices[i * 6 + 5] = fcolor.a;
    }

    if (vert_count > 4) {
        draw_mode_ = draw_mode::triangle_fan;
    } else {
        draw_mode_ = draw_mode::triangles;
    }

    if (vert_count == 4) {
        uint32 indices[] = {0, 1, 3, 1, 2, 3};

        if (primitive_indices_->get_index_count() != 6) {
            primitive_indices_->resize(6);
        }
        primitive_indices_->fill_data(0, 6, indices);
    }

    primitive_vertices_->resize(vert_count);
    primitive_vertices_->fill_data(0, vert_count, vertices.data());

    if (vert_count == 4) {
        draw_indexed(*primitive_vertices_, *primitive_indices_);
    } else {
        draw(*primitive_vertices_);
    }
}

component* renderer::create_component_(uint type_hash)
{
    component* comp = nullptr;
    if (type_hash == sprite::get_static_type_hash()) {
        comp = new sprite(context_);
    }
    if (type_hash == sprite_batch::get_static_type_hash()) {
        comp = new sprite_batch(context_);
    }
    if (type_hash == text::get_static_type_hash()) {
        comp = new text(context_);
    }
    if (type_hash == shape2D::get_static_type_hash()) {
        comp = new shape2D(context_);
    }
    drawables_.push_back(static_cast<drawable*>(comp));
    return comp;
}

bool renderer::handles_component_(uint type_hash)
{
    if (type_hash == sprite::get_static_type_hash() ||
        type_hash == text::get_static_type_hash() ||
        type_hash == shape2D::get_static_type_hash() ||
        type_hash == sprite_batch::get_static_type_hash())
    {
        return true;
    }
    return false;
}

}
