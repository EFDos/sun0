/*************************************************************************/
/*  opengl/renderer.cpp                                                  */
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
#include "common/opengl.hpp"
#include "common/types.hpp"

#include "graphics/drawable.hpp"

#include "shader.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"

#include "core/logger.hpp"
#include "core/filesys/filesys.hpp"

namespace sun {
namespace opengl {

renderer::renderer() : base_vao_(0), default_flat_shader_(nullptr) {}

renderer::~renderer() {}

bool renderer::init()
{
    auto error = glewInit();

    if (error != GLEW_NO_ERROR) {
        sun_logf_error("GLEW Initialization error: %s",
                glewGetErrorString(error));
        return false;
    }

    sun_log_info("OpenGL Initialized");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    default_flat_shader_ = create_shader("res/flat.glsl");
    default_textured_shader_ = create_shader("res/textured.glsl");

    default_textured_shader_->set_uniform("tex", 0);

    glGenVertexArrays(1, &base_vao_);
    glBindVertexArray(base_vao_);

    return sun::renderer::init();
}

void renderer::shutdown()
{
    if (default_flat_shader_ != nullptr) {
        delete default_flat_shader_;
        glDeleteVertexArrays(1, &base_vao_);
    }

    sun::renderer::shutdown();
}

sun::vertex_buffer* renderer::create_vertex_buffer(uint8 vertex_size,
                                                   size_t capacity) const
{
    return dynamic_cast<sun::vertex_buffer*>(new
        opengl::vertex_buffer(vertex_size, capacity));
}

sun::index_buffer* renderer::create_index_buffer(size_t capacity) const
{
    return dynamic_cast<sun::index_buffer*>(new opengl::index_buffer(capacity));
}

sun::shader* renderer::create_shader(const std::string& path) const
{
    auto [vertex_src, fragment_src] = shader_utils::parse_source_pair(filesys::read_file(path));

    auto gl_shader = new opengl::shader(
                new opengl::shader_stage(vertex_src, shader_stage::type::vertex),
                new opengl::shader_stage(fragment_src, shader_stage::type::fragment)
            );

    gl_shader->build();

    if (gl_shader->get_status() != shader::status::ok) {
        return nullptr;
    } else {
        return dynamic_cast<sun::shader*>(gl_shader);
    }
}

sun::texture* renderer::create_texture() const
{
    return dynamic_cast<sun::texture*>(new opengl::texture());
}

void renderer::set_model_transform(const matrix4& transform)
{
    default_flat_shader_->set_uniform("model", transform);
    default_textured_shader_->set_uniform("model", transform);

    //TODO: Provisory fix
    if (current_shader_ == default_flat_shader_ || current_shader_ == default_textured_shader_) {
        current_shader_->bind();
    }
}

void renderer::set_projection(const matrix4& projection)
{
    default_flat_shader_->set_uniform("projection", projection);
    default_textured_shader_->set_uniform("projection", projection);
    //glViewport(0, 0, 1280, 720);
}

void renderer::set_shader_(const sun::shader* p_shader) const
{
    if (current_shader_ != p_shader) {
        current_shader_ = p_shader;
        current_shader_->bind();

        sun_log_info("We're setting shader and layout");
        if (current_shader_ == default_flat_shader_) {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
            glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6,
                (void*)(sizeof(float) * 2));
        }
        if (current_shader_ == default_textured_shader_) {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 8, 0);
            glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 8,
                (void*)(sizeof(float) * 2));
            glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(float) * 8,
                (void*)(sizeof(float) * 4));
        }
    }
}

void renderer::set_texture_(const sun::texture* p_texture) const
{
    if (current_texture_ != p_texture) {
        sun_log_info("We're setting and binding texture");
        current_texture_ = p_texture;
        current_texture_->bind();
    }
}

void renderer::clear(const color& col)
{
    auto colf = to_colorf(col);
    glClearColor(colf.r, colf.g, colf.b, colf.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::clear()
{
    glClearColor(clear_color_.r,
                 clear_color_.g,
                 clear_color_.b,
                 clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer::draw(const drawable& d) const
{
    d.draw((sun::renderer*)this);
}

void renderer::draw(const sun::vertex_buffer& buffer,
                    const sun::shader* p_shader) const
{
    buffer.bind();

    if (p_shader != nullptr) {
        p_shader->bind();
    } else {
        default_flat_shader_->bind();
    }

    glDrawArrays(GL_TRIANGLES, 0, buffer.get_vertex_count());
}

void renderer::draw(const sun::vertex_buffer& buffer,
                    const sun::texture* p_texture,
                    const sun::shader* p_shader) const
{
    buffer.bind();

    if (p_shader != nullptr) {
        set_shader_(p_shader);
    } else {
        set_shader_(default_textured_shader_);
    }
    if (p_texture != nullptr) {
        set_texture_(p_texture);
    }

    glDrawArrays(GL_TRIANGLES, 0, buffer.get_vertex_count());
}

void renderer::draw_indexed(const sun::vertex_buffer& vbuffer,
                            const sun::index_buffer& ibuffer,
                            const sun::shader* p_shader) const
{
    ibuffer.bind();
    vbuffer.bind();

    if (p_shader != nullptr) {
        p_shader->bind();
    } else {
        default_flat_shader_->bind();
    }

    glDrawElements(GL_TRIANGLES, ibuffer.get_index_count(), GL_UNSIGNED_INT, 0);
}

void renderer::draw_indexed(const sun::vertex_buffer& vbuffer,
                            const sun::index_buffer& ibuffer,
                            const sun::texture* p_texture,
                            const sun::shader* p_shader) const
{
    ibuffer.bind();
    vbuffer.bind();

    if (p_shader != nullptr) {
        set_shader_(p_shader);
    } else {
        set_shader_(default_textured_shader_);
    }
    if (p_texture != nullptr) {
        set_texture_(p_texture);
    }

    glDrawElements(GL_TRIANGLES, ibuffer.get_index_count(), GL_UNSIGNED_INT, 0);
}

uint renderer::get_texture_max_size() const
{
    static bool checked = false;
    static int  size = 0;

    if(!checked){
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
    }

    return size;
}

} // opengl
} // sun
