/*************************************************************************/
/*  renderer.hpp                                                         */
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

#include "system/system.hpp"
#include "common/color.hpp"
#include "math/rect.hpp"

#include <vector>

namespace sun {

class context;

class drawable;
class camera;
class matrix4;
class vertex_buffer;
class index_buffer;
class shader;
class texture;
class font;

class SUN_API renderer : public system
{
public:

    SUN_SYSTEM_TYPE(SYS_RENDERER);

    enum class draw_mode {
        points,
        lines,
        triangles,
        triangle_fan
    };

    struct info
    {
        std::string name;
        std::string version;
        std::string vendor;
        std::string device;
    };

    struct stats
    {
        int draw_calls;
        int canvas_switches;
        int shader_switches;
        int canvases;
    };

    virtual ~renderer() = default;

    // implements system
    virtual bool init() override;

    virtual void shutdown() override;

    virtual void update() override;

    virtual vertex_buffer* create_vertex_buffer(uint8 vertex_size, size_t capacity) const = 0;

    virtual shader* create_shader(const std::string& path) const = 0;

    virtual index_buffer* create_index_buffer(size_t capacity) const = 0;

    virtual texture* create_texture() const = 0;

    virtual void clear(const color&) = 0;

    virtual void clear() = 0;

    // Drawable draw-call
    virtual void draw(const drawable&) const = 0;

    // Low-level draw-calls
    virtual void draw(const vertex_buffer& buffer,
                      const shader* p_shader = nullptr) const = 0;

    virtual void draw(const vertex_buffer& buffer,
                      const texture* p_texture,
                      const shader* p_shader = nullptr) const = 0;

    virtual void draw_indexed(const vertex_buffer& vbuffer,
                              const index_buffer& ibuffer,
                              const shader* p_shader = nullptr) const = 0;

    virtual void draw_indexed(const vertex_buffer& vbuffer,
                              const index_buffer& ibuffer,
                              const texture* p_texture,
                              const shader* p_shader = nullptr) const = 0;

    // Utility draw-calls
    virtual void draw_rect(const rectf& rect, const color&) const;

    virtual void draw_circle(const vector2f& pos, float radius, int verts) const;

    virtual void draw_line(const vector2f& begin, const vector2f& end, const color&) const;

    virtual void draw_polygon(uint vert_count,
                              const vector2f* vertices,
                              const color&) const;

    virtual void set_model_transform(const matrix4& transform) = 0;

    virtual void set_projection(const matrix4& projection) = 0;

    virtual void set_camera_transform(const matrix4& camera) = 0;

    virtual void set_viewport(const rectf& viewport);

    virtual void set_color(const color&);

    virtual uint get_texture_max_size() const = 0;

    inline void set_draw_mode(draw_mode mode) {
        draw_mode_ = mode;
    }

    inline draw_mode get_draw_mode() const {
        return draw_mode_;
    }

protected:

    explicit renderer(context&);

    virtual void set_shader_(const shader*) const = 0;

    virtual void set_texture_(const texture*) const = 0;

    rectf                       viewport_;
    colorf                      clear_color_;
    mutable draw_mode           draw_mode_;

    vertex_buffer*              primitive_vertices_;
    index_buffer*               primitive_indices_;

    mutable const shader*       current_shader_;
    mutable const texture*      current_texture_;

private:

    component* create_component_(uint type_hash) override;

    bool handles_component_(uint type_hash) override;

    std::vector<camera*>   cameras_;
    std::vector<drawable*>  drawables_;
};

}
