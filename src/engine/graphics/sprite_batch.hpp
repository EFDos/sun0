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

class Texture;

class SUN_API SpriteBatch final : public Drawable
{
public:

    SUN_COMPONENT_TYPE(SpriteBatch)

    SpriteBatch(Context& context);

    ~SpriteBatch();

    void init() override;

    void draw(Renderer* r) const override;

    void set_texture(const Texture* tex);

    void add_sprite_rect(const Vector2f& position,
                         const Recti& rect,
                         const Color& c = sun::Color::WHITE);

private:

    void update_geometry_() override;

    uint vertex_offset_;
    uint index_offset_;

    VertexBuffer*   vertices_;
    IndexBuffer*    indices_;
    const Texture*  texture_;
};

}
