/*************************************************************************/
/*  sprite_batch.hpp                                                     */
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

#include "common/types.hpp"

#include "drawable.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace sun {

class texture;
class renderer;

class SUN_API sprite_batch final : public drawable
{
public:

    SUN_COMPONENT_TYPE(sprite_batch)

    sprite_batch(context& p_context);

    ~sprite_batch();

    void draw(renderer* r) const override;

    void set_texture(const texture* tex);

    void add_sprite_rect(const vector2f& position,
                         const recti& rect,
                         const color& c = sun::color::white);

private:

    void update_geometry_() override;

    uint vertex_offset_;
    uint index_offset_;

    vertex_buffer*  vertices_;
    index_buffer*   indices_;
    const texture*  texture_;
};

}
