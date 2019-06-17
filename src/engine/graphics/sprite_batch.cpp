/*************************************************************************/
/*  sprite_batch_batch.cpp                                                     */
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
#include "sprite_batch.hpp"

#include "core/context.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"
#include "texture.hpp"

namespace sun {

SpriteBatch::SpriteBatch(Context& context)
:   Drawable(context),
    vertex_offset_(0),
    index_offset_(0),
    vertices_(nullptr),
    indices_(nullptr),
    texture_(nullptr)
{
    auto r = context_.get_system<Renderer>();
    // For now we'll use a fixed maximum of 1024 sprite_batchs
    vertices_ = r->create_vertex_buffer(sizeof(float) * 8, 4 * 1024);
    indices_ = r->create_index_buffer(6 * 1024);
}

SpriteBatch::~SpriteBatch()
{
    delete vertices_;
    delete indices_;
}

void SpriteBatch::draw(Renderer* r) const
{
    if (owning_entity_ != nullptr) {
        r->set_model_transform(owning_entity_->get_global_transform());
    }
    r->draw_indexed(*vertices_, *indices_, texture_, nullptr);
}

void SpriteBatch::set_texture(const Texture* tex)
{
    if (tex == nullptr) {
        return;
    }

    texture_ = tex;
    vertices_->resize(4 * 1024);
    indices_->resize(6 * 1024);
    vertex_offset_ = 0;
    index_offset_ = 0;
}

void SpriteBatch::add_sprite_rect(const Vector2f& pos,
                                   const Recti& p_rect,
                                   const Color& p_color)
{
    struct vertex_def {
        float x, y;
        float u, v;
        float r, g, b, a;
    };

    vertex_def quad_verts[] = {
        {
            pos.x,
            pos.y,
            (float)p_rect.x / (float)texture_->get_size().w,
            (float)p_rect.y / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            pos.x + (float)p_rect.w,
            pos.y,
            (float)p_rect.w / (float)texture_->get_size().w,
            (float)p_rect.y / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            pos.x + (float)p_rect.w,
            pos.y +(float)p_rect.h,
            (float)p_rect.w / (float)texture_->get_size().w,
            (float)p_rect.h / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            pos.x,
            pos.y + (float)p_rect.h,
            (float)p_rect.x / (float)texture_->get_size().w,
            (float)p_rect.h / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
    };

    uint indices_data[] = {
        0 + index_offset_ / 2 + index_offset_ / 6,
        1 + index_offset_ / 2 + index_offset_ / 6,
        3 + index_offset_ / 2 + index_offset_ / 6,
        1 + index_offset_ / 2 + index_offset_ / 6,
        2 + index_offset_ / 2 + index_offset_ / 6,
        3 + index_offset_ / 2 + index_offset_ / 6
    };

    vertices_->fill_data(vertex_offset_, 4, quad_verts);
    indices_->fill_data(index_offset_, 6, indices_data);
    vertex_offset_ += 4;
    index_offset_ += 6;
}

void SpriteBatch::update_geometry_()
{}

}

