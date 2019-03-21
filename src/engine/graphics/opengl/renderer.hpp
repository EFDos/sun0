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

class shader;

class SUN_API renderer final : public sun::renderer
{
public:

    renderer();

    ~renderer();

    bool init() override;

    void shutdown() override;

    sun::vertex_buffer* create_vertex_buffer(uint8 vertex_size, size_t capacity) const override;

    sun::shader* create_shader(const std::string& path) const override;

    sun::index_buffer* create_index_buffer(size_t capacity) const override;

    sun::texture* create_texture() const override;

    //font* create_font() const override;

    void clear(const color&) override;

    void clear() override;

    void set_model_transform(const matrix4& transform) override;

    void set_projection(const matrix4& projection) override;

    void draw(const drawable& d) const override;

    void draw(const sun::vertex_buffer& buffer,
              const sun::shader* p_shader = nullptr) const override;

    void draw(const sun::vertex_buffer& buffer,
              const sun::texture* p_texture = nullptr,
              const sun::shader* p_shader = nullptr) const override;

    void draw_indexed(const sun::vertex_buffer& vbuffer,
                      const sun::index_buffer& ibuffer,
                      const sun::shader* p_shader = nullptr) const override;

    void draw_indexed(const sun::vertex_buffer& vbuffer,
                      const sun::index_buffer& ibuffer,
                      const sun::texture* p_texture = nullptr,
                      const sun::shader* p_shader = nullptr) const override;

    uint get_texture_max_size() const override;

private:

    void set_shader_(const sun::shader*) const override;

    void set_texture_(const sun::texture*) const override;

    uint            base_vao_;
    sun::shader*    default_flat_shader_;
    sun::shader*    default_textured_shader_;

};

}
}
