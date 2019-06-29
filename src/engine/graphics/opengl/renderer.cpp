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
#include "framebuffer.hpp"
#include "texture.hpp"

#include "core/logger.hpp"
#include "core/filesys/filesys.hpp"

namespace sun {
namespace opengl {

constexpr GLenum get_gl_type(Renderer::DrawMode mode)
{
    switch (mode) {
        case Renderer::DrawMode::Triangles:    return GLenum(GL_TRIANGLES);
        case Renderer::DrawMode::Lines:        return GLenum(GL_LINES);
        case Renderer::DrawMode::Points:       return GLenum(GL_POINTS);
        case Renderer::DrawMode::TriangleFan:  return GLenum(GL_TRIANGLE_FAN);
        default: return GLenum();
    }
}

constexpr GLenum get_gl_type(Renderer::BlendMode mode)
{
    switch (mode) {
        case Renderer::BlendMode::SourceAlpha:
            return GLenum(GL_SRC_ALPHA);
        case Renderer::BlendMode::OneMinusSourceAlpha:
            return GLenum(GL_ONE_MINUS_SRC_ALPHA);
        case Renderer::BlendMode::Multiply:
            //return GLenum(GL_SRC_);
            return GLenum();
        default:
            return GLenum();
    }
}

Renderer::Renderer(Context& context)
:   sun::Renderer(context),
    base_vao_(0),
    default_flat_shader_(nullptr),
    default_textured_shader_(nullptr)
{
}

Renderer::~Renderer() {}

bool Renderer::init()
{
    auto error = glewInit();

    if (error != GLEW_NO_ERROR) {
        sun_logf_error("GLEW Initialization error: %s",
                glewGetErrorString(error));
        return false;
    }

    sun_log_info("OpenGL Initialized");

    //glEnable(GL_DEPTH_TEST);

    default_flat_shader_ = create_shader("res/flat.glsl");
    default_textured_shader_ = create_shader("res/textured.glsl");
    light_shader_ = create_shader("res/light.glsl");

    if (default_textured_shader_ != nullptr) {
        default_textured_shader_->send("tex", 0);
    }

    glGenVertexArrays(1, &base_vao_);
    glBindVertexArray(base_vao_);

    return sun::Renderer::init();
}

void Renderer::shutdown()
{
    if (default_flat_shader_ != nullptr) {
        delete default_flat_shader_;
    }

    if (default_textured_shader_ != nullptr) {
        delete default_textured_shader_;
    }

    glDeleteVertexArrays(1, &base_vao_);

    sun::Renderer::shutdown();
}

sun::Framebuffer* Renderer::create_framebuffer(Framebuffer::Target target) const
{
    return dynamic_cast<sun::Framebuffer*>(new opengl::Framebuffer(target));
}

sun::VertexBuffer* Renderer::create_vertex_buffer(uint8 vertex_size,
                                                   size_t capacity) const
{
    return dynamic_cast<sun::VertexBuffer*>(new
        opengl::VertexBuffer(vertex_size, capacity));
}

sun::IndexBuffer* Renderer::create_index_buffer(size_t capacity) const
{
    return dynamic_cast<sun::IndexBuffer*>(new opengl::IndexBuffer(capacity));
}

sun::Shader* Renderer::create_shader(const std::string& path) const
{
    auto [vertex_src, fragment_src] = ShaderUtils::parse_source_pair(filesys::read_file(path));

    auto gl_Shader = new opengl::Shader(
                new opengl::ShaderStage(vertex_src, ShaderStage::Type::Vertex),
                new opengl::ShaderStage(fragment_src, ShaderStage::Type::Fragment)
            );

    gl_Shader->build();

    if (gl_Shader->get_status() != Shader::Status::Ok) {
        return nullptr;
    } else {
        return dynamic_cast<sun::Shader*>(gl_Shader);
    }
}

sun::Texture* Renderer::create_texture() const
{
    return dynamic_cast<sun::Texture*>(new opengl::Texture(context_));
}

int Renderer::add_light(const Vector2f& pos, const Color& col, float intensity) const
{
    auto idx_str = std::to_string(++light_count_);
    light_shader_->send("lights[" + idx_str + "].pos", pos);
    light_shader_->send("lights[" + idx_str + "].intensity", intensity);
    return light_count_;
}

void Renderer::set_blend_mode(BlendMode source, BlendMode dest)
{
    glEnable(GL_BLEND);
    glBlendFunc(get_gl_type(source), get_gl_type(dest));
    sun::Renderer::set_blend_mode(source, dest);
}

void Renderer::set_model_transform(const Matrix4& transform)
{
    if (default_flat_shader_ == nullptr || default_textured_shader_ == nullptr) {
        return;
    }
    default_flat_shader_->send("model", transform);
    default_textured_shader_->send("model", transform);

    //TODO: Provisory fix, cause send unbinds Shader
    if (current_shader_ == default_flat_shader_ || current_shader_ == default_textured_shader_) {
        current_shader_->bind();
    }
}

void Renderer::set_viewport(const Rectf& viewport)
{
    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
    sun::Renderer::set_viewport(viewport);
}

void Renderer::set_projection(const Matrix4& projection)
{
    if (default_flat_shader_ == nullptr || default_textured_shader_ == nullptr) {
        return;
    }
    default_flat_shader_->send("projection", projection);
    default_textured_shader_->send("projection", projection);
    light_shader_->send("projection", projection);
}

void Renderer::set_camera_transform(const Matrix4& transform)
{
    if (default_flat_shader_ == nullptr || default_textured_shader_ == nullptr) {
        return;
    }
    default_flat_shader_->send("viewport", transform);
    default_textured_shader_->send("viewport", transform);
    light_shader_->send("viewport", transform);

    //TODO: Provisory fix, cause send unbinds Shader
    if (current_shader_ == default_flat_shader_ || current_shader_ == default_textured_shader_) {
        current_shader_->bind();
    }
}

void Renderer::set_shader_(const sun::Shader* shader) const
{
    if (current_shader_ != shader) {
        current_shader_ = shader;
        if (current_shader_ == nullptr) {
            return;
        }
        current_shader_->bind();

        if (current_shader_ == default_flat_shader_) {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
        if (current_shader_ == default_textured_shader_) {
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
        }
    }
}

void Renderer::set_texture_(const sun::Texture* texture) const
{
    if (current_texture_ != texture) {
        current_texture_ = texture;
        if (current_texture_ == nullptr){
            return;
        }
    }
    current_texture_->bind();
}

void Renderer::clear(const Color& col)
{
    auto colf = Color::to_colorf(col);
    glClearColor(colf.r, colf.g, colf.b, colf.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clear()
{
    glClearColor(clear_color_.r,
                 clear_color_.g,
                 clear_color_.b,
                 clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const Drawable& drawable) const
{
    drawable.draw((sun::Renderer*)this);
}

void Renderer::draw(const sun::VertexBuffer& buffer,
                    const sun::Shader* shader) const
{
    buffer.bind();

    if (shader != nullptr) {
        set_shader_(shader);
    } else {
        set_shader_(default_flat_shader_);
    }
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6,
        (void*)(sizeof(float) * 2));

    glDrawArrays(get_gl_type(draw_mode_), 0, buffer.get_vertex_count());
}

void Renderer::draw(const sun::VertexBuffer& buffer,
                    const sun::Texture* texture,
                    const sun::Shader* shader) const
{
    buffer.bind();

    if (shader != nullptr) {
        set_shader_(shader);
    } else {
        set_shader_(default_textured_shader_);
    }
    if (texture != nullptr) {
        set_texture_(texture);
    }
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 8, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 8,
        (void*)(sizeof(float) * 2));
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(float) * 8,
        (void*)(sizeof(float) * 4));

    glDrawArrays(get_gl_type(draw_mode_), 0, buffer.get_vertex_count());
}

void Renderer::draw_indexed(const sun::VertexBuffer& vbuffer,
                            const sun::IndexBuffer& ibuffer,
                            const sun::Shader* shader) const
{
    ibuffer.bind();
    vbuffer.bind();

    if (shader != nullptr) {
        set_shader_(shader);
    } else {
        set_shader_(default_flat_shader_);
    }
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 6, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 6,
        (void*)(sizeof(float) * 2));

    glDrawElements(get_gl_type(draw_mode_),
                   ibuffer.get_index_count(),
                   GL_UNSIGNED_INT, 0);
}

void Renderer::draw_indexed(const sun::VertexBuffer& vbuffer,
                            const sun::IndexBuffer& ibuffer,
                            const sun::Texture* texture,
                            const sun::Shader* shader) const
{
    ibuffer.bind();
    vbuffer.bind();

    if (shader != nullptr) {
        set_shader_(shader);
    } else {
        set_shader_(default_textured_shader_);
    }
    if (texture != nullptr) {
        set_texture_(texture);
    }
    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(float) * 8, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(float) * 8,
        (void*)(sizeof(float) * 2));
    glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(float) * 8,
        (void*)(sizeof(float) * 4));

    glDrawElements(get_gl_type(draw_mode_),
                   ibuffer.get_index_count(),
                   GL_UNSIGNED_INT, 0);
}

uint Renderer::get_texture_max_size() const
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
