/*************************************************************************/
/*  sprite.cpp                                                           */
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
#include "sprite.hpp"

#include "core/context.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"
#include "texture.hpp"

namespace sun {

sprite::sprite(context& p_context)
:   drawable(p_context),
    vertices_(nullptr),
    indices_(nullptr),
    texture_(nullptr)
{
    auto r = context_.get_system<renderer>();
    vertices_ = r->create_vertex_buffer(sizeof(float) * 8, 4);
    indices_ = r->create_index_buffer(6);
}

sprite::~sprite()
{
    delete vertices_;
    delete indices_;
}

void sprite::draw(renderer* r) const
{
    if (owning_entity_ != nullptr) {
        r->set_model_transform(owning_entity_->get_global_transform());
    }
    r->set_draw_mode(renderer::draw_mode::triangles);
    r->draw_indexed(*vertices_, *indices_, texture_, nullptr);
}

void sprite::set_texture(const texture* tex)
{
    if (tex == nullptr) {
        return;
    }

    texture_ = tex;

    if (rect_.x == 0 && rect_.y == 0 && rect_.w == 0 && rect_.h == 0) {
        rect_.set_size(texture_->get_size().x, texture_->get_size().y);
    }

    update_geometry_();
}

void sprite::update_geometry_()
{
    struct vertex_def {
        float x, y;
        float u, v;
        float r, g, b, a;
    };

    vertex_def quad_verts[] = {
        {
            0.f ,
            0.f,
            (float)rect_.x / (float)texture_->get_size().w,
            (float)rect_.y / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            (float)rect_.w,
            0.f,
            (float)rect_.w / (float)texture_->get_size().w,
            (float)rect_.y / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            (float)rect_.w,
            (float)rect_.h,
            (float)rect_.w / (float)texture_->get_size().w,
            (float)rect_.h / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            0.f,
            (float)rect_.h,
            (float)rect_.x / (float)texture_->get_size().w,
            (float)rect_.h / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
    };

    uint indices_data[] = {
        0, 1, 3,
        1, 2, 3
    };

    vertices_->fill_data(0, 4, quad_verts);
    indices_->fill_data(0, 6, indices_data);
}

}
