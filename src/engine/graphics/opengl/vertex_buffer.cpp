/*************************************************************************/
/*  opengl/vertex_buffer.cpp                                             */
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
#include "vertex_buffer.hpp"

#include "common/opengl.hpp"
#include "core/logger.hpp"

namespace sun {
namespace opengl {

VertexBuffer::VertexBuffer(uint8 vertex_size, size_t capacity)
:   sun::VertexBuffer(vertex_size, capacity), vbo_(0)
{
    glGenBuffers(1, &vbo_);
    resize(capacity);
}

VertexBuffer::~VertexBuffer()
{
    release();
}

void VertexBuffer::release()
{
    if (vbo_ != 0) {
        glDeleteBuffers(GL_ARRAY_BUFFER, &vbo_);
        vertex_count_ = 0;
        capacity_ = 0;
    }
}

void VertexBuffer::fill_data(size_t offset, size_t count, const void *data)
{
    if (vbo_ == 0) {
        sun_log_error("Failed to fill vertex buffer: Buffer in invalid state.");
        return;
    }

    if (offset + count > capacity_) {
        sun_log_error("Failed to fill vertex buffer data: over capacity");
        return;
    }

    if (count + offset > vertex_count_) {
        vertex_count_ += count;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, offset * vertex_size_,
        count * vertex_size_, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::resize(size_t capacity)
{
    if (vbo_ == 0) {
        sun_log_error("Failed to resize vertex buffer:"
                      " Buffer in invalid state.");
        return;
    }

    vertex_count_ = 0;
    capacity_ = capacity;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, capacity_ * vertex_size_, nullptr,
        dynamic_ ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::clear()
{
    if (vbo_ == 0) {
        sun_log_error("Failed to clear vertex buffer:"
                      " Buffer in invalid state.");
        return;
    }

    vertex_count_ = 0;
    capacity_ = 0;
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr,
        dynamic_ ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::bind() const
{
    if (vbo_ == 0) {
        sun_log_error("Failed to bind vertex buffer: Buffer in invalid state.");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::set_dynamic(bool dynamic)
{
    if (vbo_ == 0) {
        sun_log_error("Failed to set property on vertex buffer:"
                      "Buffer in invalid state.");
        return;
    }

    if (dynamic_ != dynamic && vertex_count_ > 0) {
        // get existing data
        size_t data_size = capacity_ * vertex_size_;
        uint8* temp_data = new uint8[data_size];

        glGetBufferSubData(GL_ARRAY_BUFFER, 0, data_size, temp_data);

        // resize will reset the buffer usage as required
        resize(capacity_);

        // get data back to buffer
        fill_data(0, capacity_, temp_data);

        delete [] temp_data;
    }

    sun::VertexBuffer::set_dynamic(dynamic);
}

} // opengl
} // sun
