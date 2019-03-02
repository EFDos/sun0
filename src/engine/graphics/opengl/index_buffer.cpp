/*************************************************************************/
/*  opengl/index_buffer.cpp                                              */
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
#include "index_buffer.hpp"

#include "common/opengl.hpp"
#include "core/logger.hpp"

namespace sun {
namespace opengl {

index_buffer::index_buffer(size_t capacity)
:   sun::index_buffer(capacity), ibo_(0)
{
    glGenBuffers(1, &ibo_);
    resize(capacity);
}

index_buffer::~index_buffer()
{
    release();
}

void index_buffer::release()
{
    if (ibo_ != 0) {
        glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &ibo_);
        index_count_ = 0;
        capacity_ = 0;
    }
}

void index_buffer::fill_data(size_t offset, size_t count, const uint* data)
{
    if (offset + count > capacity_) {
        sun_log_error("Failed to fill index buffer data: over capacity");
        return;
    }

    index_count_ += count;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(uint),
        count * sizeof(uint), data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::resize(size_t capacity)
{
    index_count_ = 0;
    capacity_ = capacity;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, capacity_ * sizeof(uint), nullptr,
        GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::clear()
{
    capacity_ = 0;
    index_count_ = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void index_buffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
}

void index_buffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // opengl
} // sun
