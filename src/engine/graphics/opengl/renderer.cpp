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

#include "shader.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

#include "core/logger.hpp"
#include "core/filesys/filesys.hpp"

namespace sun {
namespace opengl {

renderer::renderer() : flat_vao_(0), default_flat_shader_(nullptr) {}

renderer::~renderer() {}

void renderer::init()
{
    sun::renderer::init();

    auto error = glewInit();

    if (error != GLEW_NO_ERROR) {
        sun_logf_error("GLEW Initialization error: %s",
                glewGetErrorString(error));
        return;
    }

    sun_log_info("OpenGL Initialized");

    default_flat_shader_ = create_shader("res/flat.glsl");

    glGenVertexArrays(1, &flat_vao_);
    glBindVertexArray(flat_vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 2));
}

void renderer::shutdown()
{
    if (default_flat_shader_ != nullptr) {
        delete default_flat_shader_;
        glDeleteVertexArrays(1, &flat_vao_);
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

void renderer::draw(const sun::vertex_buffer& buffer,
                    const sun::shader* p_shader) const
{
    if (p_shader != nullptr) {
        p_shader->bind();
    } else {
        default_flat_shader_->bind();
    }
    glBindVertexArray(flat_vao_);
    buffer.bind();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 2));

    glDrawArrays(GL_TRIANGLES, 0, buffer.get_vertex_count());
}

void renderer::draw_indexed(const sun::vertex_buffer& vbuffer,
                            const sun::index_buffer& ibuffer,
                            const sun::shader* p_shader) const
{
    if (p_shader != nullptr) {
        p_shader->bind();
    } else {
        default_flat_shader_->bind();
    }

    glBindVertexArray(flat_vao_);
    vbuffer.bind();
    ibuffer.bind();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6, (void*)(sizeof(float) * 2));

    glDrawElements(GL_TRIANGLES, ibuffer.get_index_count(), GL_UNSIGNED_INT, nullptr);
}

} // opengl
} // sun
