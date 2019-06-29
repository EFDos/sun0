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
#include "framebuffer.hpp"

#include <vector>

namespace sun {

class Context;

class Drawable;
class Camera;
class Matrix4;
class VertexBuffer;
class IndexBuffer;
class Shader;
class Texture;
class Font;

class SUN_API Renderer : public System
{
public:

    SUN_SYSTEM_TYPE(Renderer);

    enum class DrawMode {
        Points,
        Lines,
        Triangles,
        TriangleFan
    };

    enum class BlendMode {
        SourceAlpha,
        OneMinusSourceAlpha,
        Multiply
    };

    //TODO Implement Info
    struct Info
    {
        std::string Name;
        std::string Version;
        std::string Vendor;
        std::string Device;
    };

    //TODO Implement Stats
    struct Stats
    {
        int DrawCalls;
        int CanvasSwitches;
        int ShaderSwitches;
        int Canvases;
    };

    virtual ~Renderer() = default;

    // implements system
    virtual bool init() override;

    virtual void shutdown() override;

    virtual void render();

    virtual Framebuffer* create_framebuffer(Framebuffer::Target target) const = 0;

    virtual VertexBuffer* create_vertex_buffer(uint8 vertex_size, size_t capacity) const = 0;

    virtual Shader* create_shader(const std::string& path) const = 0;

    virtual IndexBuffer* create_index_buffer(size_t capacity) const = 0;

    virtual Texture* create_texture() const = 0;

    int add_light(const Vector2f& pos, const Colorf& col, float intensity) const;

    virtual void clear(const Color&) = 0;

    virtual void clear() = 0;

    // Drawable draw-call
    virtual void draw(const Drawable&) const = 0;

    // Low-level draw-calls
    virtual void draw(const VertexBuffer& buffer,
                      const Shader* p_shader = nullptr) const = 0;

    virtual void draw(const VertexBuffer& buffer,
                      const Texture* p_texture,
                      const Shader* p_shader = nullptr) const = 0;

    virtual void draw_indexed(const VertexBuffer& vbuffer,
                              const IndexBuffer& ibuffer,
                              const Shader* p_shader = nullptr) const = 0;

    virtual void draw_indexed(const VertexBuffer& vbuffer,
                              const IndexBuffer& ibuffer,
                              const Texture* p_texture,
                              const Shader* p_shader = nullptr) const = 0;

    // Utility draw-calls
    virtual void draw_rect(const Rectf& rect, const Color&) const;

    virtual void draw_circle(const Vector2f& pos, float radius, int verts) const;

    virtual void draw_line(const Vector2f& begin, const Vector2f& end, const Color&) const;

    virtual void draw_polygon(uint vert_count,
                              const Vector2f* vertices,
                              const Color&) const;

    virtual void set_model_transform(const Matrix4& transform) = 0;

    virtual void set_projection(const Matrix4& projection) = 0;

    virtual void set_camera_transform(const Matrix4& camera) = 0;

    virtual void set_viewport(const Rectf& viewport);

    virtual void set_color(const Color&);

    virtual void set_blend_mode(BlendMode source, BlendMode dest);

    virtual uint get_texture_max_size() const = 0;

    inline void set_draw_mode(DrawMode mode) {
        draw_mode_ = mode;
    }

    inline DrawMode get_draw_mode() const {
        return draw_mode_;
    }

    inline BlendMode get_source_blend_mode() const {
        return blend_src_;
    }

    inline BlendMode get_dest_blend_mode() const {
        return blend_dest_;
    }

protected:

    explicit Renderer(Context&);

    virtual void set_shader_(const Shader*) const = 0;

    virtual void set_texture_(const Texture*) const = 0;

    Rectf                       viewport_;
    Colorf                      clear_color_;
    mutable BlendMode           blend_src_;
    mutable BlendMode           blend_dest_;
    mutable DrawMode            draw_mode_;

    VertexBuffer*               primitive_vertices_;
    IndexBuffer*                primitive_indices_;

    VertexBuffer*               screen_quad_buffer_;
    IndexBuffer*                screen_quad_indices_;

    Framebuffer*                screen_buffer_;
    Texture*                    screen_buffer_texture_;

    Shader*                     light_shader_;

    mutable int                         light_count_;

    mutable const Shader*       current_shader_;
    mutable const Texture*      current_texture_;

private:

    Component* create_component_(uint type_hash, uint id) override;

    bool handles_component_(uint type_hash) override;

    std::vector<Camera*>   cameras_;
    std::vector<Drawable*>  drawables_;
};

}
