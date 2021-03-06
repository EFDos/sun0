/*************************************************************************/
/*  index_buffer.hpp                                                     */
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

#include "common/int.hpp"
#include "gpu_object.hpp"

namespace sun {

class IndexBuffer : public GPUObject
{
public:

    virtual ~IndexBuffer() = default;

    virtual void fill_data(size_t offset, size_t count, const uint* data) = 0;

    virtual void resize(size_t count) = 0;

    virtual void clear() = 0;

    virtual void bind() const = 0;

    virtual void unbind() const = 0;

    size_t get_index_count() const { return index_count_; }

    size_t get_capacity() const { return capacity_; }

protected:

    IndexBuffer(size_t capacity);

    size_t  index_count_;
    size_t  capacity_;
};

}
