/*************************************************************************/
/*  vertex_buffer.hpp                                                    */
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

#include "vertex_layout.hpp"
#include "gpu_object.hpp"

namespace sun {

class VertexBuffer : public GPUObject
{
public:

    virtual ~VertexBuffer() = default;

    virtual void fill_data(size_t offset, size_t count, const void* data) = 0;

    virtual void resize(size_t capacity) = 0;

    virtual void clear() = 0;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    void set_layout(const VertexLayout& layout) {
        layout_ = layout;
    }

    virtual void set_dynamic(bool dynamic) { dynamic_ = dynamic; }

    bool is_dynamic() const { return dynamic_; }

    const VertexLayout& get_layout() const {
        return layout_;
    }

    uint8 get_vertex_size() const { return vertex_size_; }

    size_t get_vertex_count() const { return vertex_count_; }

    size_t get_capacity() const { return capacity_; }

protected:

    VertexBuffer(uint8 vertex_size, size_t capacity, bool dynamic = false);

    VertexLayout   layout_;
    uint8           vertex_size_;
    size_t          vertex_count_;
    size_t          capacity_;
    bool            dynamic_;
};

}
