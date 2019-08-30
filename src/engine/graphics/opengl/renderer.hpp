/*************************************************************************/
/*  opengl/renderer.hpp                                                  */
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

#include "graphics/renderer.hpp"

namespace sun {
namespace opengl {

class Shader;

class SUN_API Renderer final : public sun::Renderer
{
public:

    Renderer(Context&);

    ~Renderer();

    bool init() override;

    void shutdown() override;

    sun::Framebuffer* create_framebuffer(Framebuffer::Target target) const override;

    sun::VertexBuffer* create_vertex_buffer(uint8 vertex_size, size_t capacity) const override;

    sun::Shader* create_shader(const std::string& path) const override;

    sun::IndexBuffer* create_index_buffer(size_t capacity) const override;

    sun::Texture* create_texture() const override;

    void clear(const Color&) override;

    void clear() override;

    void set_viewport(const Rectf& viewport) override;

    void set_model_transform(const Matrix4& transform) override;

    void set_projection(const Matrix4& projection) override;

    void set_camera_transform(const Matrix4& transform) override;

    void set_blend_mode(BlendMode source, BlendMode dest) override;

    void draw(const Drawable& d) const override;

    void draw(const sun::VertexBuffer& buffer,
              sun::Shader* p_shader = nullptr) const override;

    void draw(const sun::VertexBuffer& buffer,
              const sun::Texture* p_texture,
              sun::Shader* p_shader = nullptr) const override;

    void draw_indexed(const sun::VertexBuffer& vbuffer,
                      const sun::IndexBuffer& ibuffer,
                      sun::Shader* p_shader = nullptr) const override;

    void draw_indexed(const sun::VertexBuffer& vbuffer,
                      const sun::IndexBuffer& ibuffer,
                      const sun::Texture* p_texture,
                      sun::Shader* p_shader = nullptr) const override;

    uint get_texture_max_size() const override;

private:

    void set_shader_(sun::Shader*) const override;

    void set_texture_(const sun::Texture*) const override;

    uint            base_vao_;
    sun::Shader*    default_flat_shader_;
    sun::Shader*    default_textured_shader_;

};

}
}
