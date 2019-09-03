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

#include "image.hpp"
#include "shader.hpp"
#include "font.hpp"

// components
#include "camera.hpp"
#include "sprite.hpp"
#include "sprite_batch.hpp"
#include "text.hpp"
#include "shape2D.hpp"
#include "light2D.hpp"

namespace sun {

Renderer::Renderer(Context& context)
:   System(context),
    draw_mode_(DrawMode::Triangles),
    primitive_vertices_(nullptr),
    primitive_indices_(nullptr),
    light_count_(0),
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

    text_quad_buffer_ = create_vertex_buffer(sizeof(float) * 8, 0);
    text_index_buffer_ = create_index_buffer(0);

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

    default_font_ = new Font(context_);
    default_font_->load("res/mono.ttf");

    sun_log_info("Graphics System ready.");
    return true;
}

void Renderer::shutdown()
{
    delete default_font_;

    drawables_.clear();
    cameras_.clear();
    lights_.clear();
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
        0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        viewport_.w, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f,
        viewport_.w, viewport_.h, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f,
        0.f, viewport_.h, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f,
    };
    screen_buffer_texture_->resize(Vector2f::to_vector2u(viewport.get_size()));
    screen_quad_buffer_->fill_data(0, 4, vertices);
}

int Renderer::add_light(const Vector2f& pos, const Colorf& col, float intensity) const
{
    auto idx_str = std::to_string(light_count_);
    light_shader_->send("lights[" + idx_str + "].pos", pos);
    light_shader_->send("lights[" + idx_str + "].intensity", intensity);
    light_shader_->send("lights[" + idx_str + "].color", col);
    return light_count_++;
}

void Renderer::update_light(int light_id, const Vector2f& pos) const
{
    if (light_id >= 0 && light_id <= light_count_) {
        auto idx_str = std::to_string(light_id);
        light_shader_->send("lights[" + idx_str + "].pos", pos);
    }
}

void Renderer::update_light(int light_id, const Colorf& color) const
{
    if (light_id >= 0 && light_id <= light_count_) {
        auto idx_str = std::to_string(light_id);
        light_shader_->send("lights[" + idx_str + "].color", color);
    }
}

void Renderer::update_light(int light_id, float intensity) const
{
    if (light_id >= 0 && light_id <= light_count_) {
        auto idx_str = std::to_string(light_id);
        light_shader_->send("lights[" + idx_str + "].intensity", intensity);
    }
}

void Renderer::update(float delta)
{
    for (auto l : lights_) {
        if (l->get_update()) {
            l->update(delta);
        }
    }
    for (auto c : cameras_) {
        if (c->get_update()) {
            c->update(delta);
        }
    }
}

void Renderer::render()
{
    clear();
    if (light_count_ > 0) {
        screen_buffer_->bind();
        clear();
    }
    for (auto d : drawables_) {
        if (d->get_draw()) {
            d->prepare_draw();
            draw(*d);
        }
    }
    if (light_count_ > 0) {
        screen_buffer_->unbind();
        draw_indexed(*screen_quad_buffer_, *screen_quad_indices_, screen_buffer_texture_, light_shader_);
    }
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

void Renderer::draw_text(const std::string& text, const Vector2f& position,
                         uint char_size, const Color& color) const
{
    for (auto c: text) {
        default_font_->get_glyph(c, char_size);
    }

    auto page_texture = default_font_->get_page_texture(char_size);

    if (page_texture == nullptr) {
        return;
    }

    text_quad_buffer_->resize(text.length() * 4);
    text_index_buffer_->resize(text.length() * 6);

    float hspace = (float)default_font_->get_glyph(' ', char_size).advance;
    float vspace = (float)default_font_->get_line_spacing(char_size);
    float x = 0.f, y = char_size;
    int offset = 0;
    int i_offset = 0;
    uint i_value_offset = 0;
    char prev_char = 0;
    for (auto c : text)
    {
        x += default_font_->get_kerning(prev_char, c, char_size);
        prev_char = c;

        if(c == ' ' || c == '\n' || c == '\t')
        {
            switch(c)
            {
                case ' ': x += hspace; break;
                case '\t': x += hspace * 4; break;
                case '\n': y += vspace; x = 0.f; break;
            }
            continue;
        }

        const Font::Glyph& g = default_font_->get_glyph(c, char_size);
        auto tex_size = page_texture->get_size();

        float pos_x = x + g.rect.x;
        float pos_y = y + g.rect.y;
        float pos_w = g.rect.w;
        float pos_h = g.rect.h;
        float tex_x = (float)g.tex_coords.x / (float)tex_size.x;
        float tex_y = (float)g.tex_coords.y / (float)tex_size.y;
        float tex_w = (float)g.tex_coords.w / (float)tex_size.x;
        float tex_h = (float)g.tex_coords.h / (float)tex_size.y;

        Colorf col = Color::to_colorf(color);

        x += g.advance + g.rect.w / 6.f;

        float vertices[] = {
            pos_x,          pos_y,
            tex_x,          tex_y,
            col.r,          col.g,  col.b,   col.a,

            pos_x + pos_w,  pos_y,
            tex_x + tex_w,  tex_y,
            col.r,          col.g,  col.b,   col.a,

            pos_x + pos_w,  pos_y + pos_h,
            tex_x + tex_w,  tex_y + tex_h,
            col.r,          col.g,  col.b,   col.a,

            pos_x,          pos_y + pos_h,
            tex_x,          tex_y + tex_h,
            col.r,          col.g,  col.b,     col.a
        };

        uint32 quad_indices[] = {
            i_value_offset + 0, i_value_offset + 1, i_value_offset + 3,
            i_value_offset + 1, i_value_offset + 2, i_value_offset + 3
        };

        text_quad_buffer_->fill_data(offset, 4, vertices);
        text_index_buffer_->fill_data(i_offset, 6, quad_indices);

        offset += 4;
        i_offset += 6;
        i_value_offset += 4;
    }

    draw_mode_ = DrawMode::Triangles;
    draw_indexed(*text_quad_buffer_, *text_index_buffer_,
        default_font_->get_page_texture(char_size), nullptr);
}

Ref<Component> Renderer::create_component_(uint type_hash, uint id, bool init)
{
    Ref<Component> comp = nullptr;
    if (type_hash == Camera::get_static_type_hash()) {
        auto cam = std::make_shared<Camera>(context_);
        cameras_.push_back(cam);
        comp = cam;
        if (init) {
            comp->init();
        }
        comp->set_id(id);
        return comp;
    }
    if (type_hash == Light2D::get_static_type_hash()) {
        auto light = std::make_shared<Light2D>(context_);
        lights_.push_back(light);
        comp = light;
        if (init) {
            comp->init();
        }
        comp->set_id(id);
        return comp;
    }
    if (type_hash == Sprite::get_static_type_hash()) {
        drawables_.push_back(std::make_shared<Sprite>(context_));
        comp = drawables_.back();
    }
    if (type_hash == SpriteBatch::get_static_type_hash()) {
        drawables_.push_back(std::make_shared<SpriteBatch>(context_));
        comp = drawables_.back();
    }
    if (type_hash == Text::get_static_type_hash()) {
        drawables_.push_back(std::make_shared<Text>(context_));
        comp = drawables_.back();
    }
    if (type_hash == Shape2D::get_static_type_hash()) {
        drawables_.push_back(std::make_shared<Shape2D>(context_));
        comp = drawables_.back();
    }
    if (init) {
        comp->init();
    }
    comp->set_id(id);
    return comp;
}

bool Renderer::handles_component_(uint type_hash)
{
    if (type_hash == Sprite::get_static_type_hash() ||
        type_hash == Text::get_static_type_hash() ||
        type_hash == Shape2D::get_static_type_hash() ||
        type_hash == Light2D::get_static_type_hash() ||
        type_hash == SpriteBatch::get_static_type_hash() ||
        type_hash == Camera::get_static_type_hash())
    {
        return true;
    }
    return false;
}

Resource* Renderer::create_resource_(uint type_hash)
{
    if (type_hash == Texture::get_static_type_hash()) {
        return create_texture();
    }
    if (type_hash == Image::get_static_type_hash()) {
        return new Image(context_);
    }
    if (type_hash == Font::get_static_type_hash()) {
        return new Font(context_);
    }
    return nullptr;
}

bool Renderer::handles_resource_(uint type_hash)
{
    if (type_hash == Texture::get_static_type_hash() ||
        type_hash == Image::get_static_type_hash() ||
        type_hash == Font::get_static_type_hash()) {
        return true;
    }
    return false;
}

}
