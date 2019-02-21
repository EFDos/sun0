/*************************************************************************/
/*  opengl/vertex_buffer.hpp                                             */
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

#include "graphics/vertex_buffer.hpp"

namespace sun {
namespace opengl {

class SUN_API vertex_buffer final : public sun::vertex_buffer
{
public:

    vertex_buffer(uint8 vertex_size, size_t capacity);

    ~vertex_buffer();

    // implements gpu_object

    void release() override;

    // implements sun::vertex_buffer

    void fill_data(size_t offset, size_t count, const void* data) override;

    void resize(size_t capacity) override;

    void clear() override;

    void bind() override;

    void unbind() override;

    void set_dynamic(bool) override;

private:

    uint    vbo_;

};

} // opengl
} // sun