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
#include "common/types.hpp"
#include "core/logger.hpp"

#include "font.hpp"

// components
#include "camera.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "text.hpp"
#include "shape2D.hpp"

namespace sun {

Renderer::Renderer(Context& context)
:   System(context),
    draw_mode_(DrawMode::Triangles),
    primitive_vertices_(nullptr),
    primitive_indices_(nullptr),
    current_shader_(nullptr),
    current_texture_(nullptr)
{
}

bool Renderer::init()
{
    primitive_vertices_ = create_vertex_buffer(sizeof(float) * 6, 0);
    primitive_indices_ = create_index_buffer(0);

    screen_quad_buffer_ = create_vertex_buffer(sizeof(float) * 8, 4);
    screen_quad_indices_ = create_index_buffer(6);

    screen_buffer_ = create_framebuffer(Framebuffer::Target::ReadAndDraw);
    screen_buffer_texture_ = create_texture();

    screen_buffer_texture_->resize({800, 600});
    screen_buffer_->attach_texture(screen_buffer_texture_);

    float vertices[] = {
        0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
        800.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
        800.f, 600.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        0.f, 600.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
    };

    uint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    screen_quad_buffer_->fill_data(0, 4, vertices);
    screen_quad_indices_->fill_data(0, 6, indices);

    set_blend_mode(BlendMode::SourceAlpha, BlendMode::OneMinusSourceAlpha);

    sun_log_info("Graphics System ready.");
    return true;
}

void Renderer::shutdown()
{
    for (auto drawable : drawables_) {
        delete drawable;
    }
    sun_log_info("Graphics System shutdown.");
}

void Renderer::set_color(const Color& col)
{
    clear_color_ = Color::to_colorf(col);
}

void Renderer::set_blend_mode(BlendMode source, BlendMode dest)
{
    blend_src_ = source;
    blend_dest_ = dest;
}

void Renderer::set_viewport(const Rectf& viewport)
{
    viewport_ = viewport;
    float vertices[] = {
        0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
        viewport_.w, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
        viewport_.w, viewport_.h, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        0.f, viewport_.h, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
    };
    screen_buffer_texture_->resize(Vector2f::to_vector2u(viewport.get_size()));
    screen_quad_buffer_->fill_data(0, 4, vertices);
}

void Renderer::render()
{
    clear();
    screen_buffer_->bind();
    clear();
    for (auto c : cameras_) {
        c->update_transform(*this);
    }
    for (auto s : drawables_) {
        draw(*s);
    }
    screen_buffer_->unbind();
    set_model_transform(Matrix4());
    set_camera_transform(Matrix4());
    draw_indexed(*screen_quad_buffer_, *screen_quad_indices_, screen_buffer_texture_);
}

void Renderer::draw_rect(const Rectf& rect, const Color& c) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    std::vector<Vector2f> vertices{
        {rect.x, rect.y},
        {rect.x + rect.w, rect.y},
        {rect.x + rect.w, rect.y + rect.h},
        {rect.x, rect.y + rect.h},
    };

    draw_polygon(4, vertices.data(), c);

    /*auto fcolor = Color::to_colorf(c);
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

    draw_mode_ = DrawMode::Triangles;

    uint32 indices[] = {0, 1, 3, 1, 2, 3};

    if (primitive_indices_->get_index_count() != 6) {
        primitive_indices_->resize(6);
    }
    primitive_indices_->fill_data(0, 6, indices);

    if (primitive_vertices_->get_vertex_count() < 4) {
        primitive_vertices_->resize(4);
    }
    primitive_vertices_->fill_data(0, 4, vertices.data());

    draw_indexed(*primitive_vertices_, *primitive_indices_);*/
}

void Renderer::draw_circle(const Vector2f& pos, float radius, int verts) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }
    //TODO implement draw circle
    sun_log_warn("Renderer::draw_circle: not yet implemented");
}

void Renderer::draw_line(const Vector2f& begin, const Vector2f& end, const Color& c) const
{
    if (primitive_vertices_ == nullptr || primitive_indices_ == nullptr) {
        sun_log_error("Graphics backend did not intialize buffers"
            " for primitives rendering");
        return;
    }

    auto fcolor = Color::to_colorf(c);
    std::vector<float> vertices{
        begin.x, begin.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
        end.x, end.y,
        fcolor.r, fcolor.g, fcolor.b, fcolor.a,
    };

    draw_mode_ = DrawMode::Lines;

    primitive_vertices_->resize(2);
    primitive_vertices_->fill_data(0, 2, vertices.data());

    draw(*primitive_vertices_);
}

void Renderer::draw_polygon(uint vert_count,
                            const Vector2f* verts,
                            const Color& c) const
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

    auto fcolor = Color::to_colorf(c);
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
        draw_mode_ = DrawMode::TriangleFan;
    } else {
        draw_mode_ = DrawMode::Triangles;
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

Component* Renderer::create_component_(uint type_hash, uint id)
{
    Component* comp = nullptr;
    if (type_hash == Camera::get_static_type_hash()) {
        Camera* cam = new Camera(context_);
        cam->set_viewport_size(viewport_.get_size());
        cameras_.push_back(cam);
        comp = cam;
        comp->set_id(id);
        return comp;
    }
    if (type_hash == Sprite::get_static_type_hash()) {
        comp = new Sprite(context_);
    }
    if (type_hash == SpriteBatch::get_static_type_hash()) {
        comp = new SpriteBatch(context_);
    }
    if (type_hash == Text::get_static_type_hash()) {
        comp = new Text(context_);
    }
    if (type_hash == Shape2D::get_static_type_hash()) {
        comp = new Shape2D(context_);
    }
    drawables_.push_back(static_cast<Drawable*>(comp));
    comp->set_id(id);
    return comp;
}

bool Renderer::handles_component_(uint type_hash)
{
    if (type_hash == Sprite::get_static_type_hash() ||
        type_hash == Text::get_static_type_hash() ||
        type_hash == Shape2D::get_static_type_hash() ||
        type_hash == SpriteBatch::get_static_type_hash() ||
        type_hash == Camera::get_static_type_hash())
    {
        return true;
    }
    return false;
}

}
