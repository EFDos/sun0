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
#include "core/logger.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"
#include "texture.hpp"

namespace sun {

Sprite::Sprite(Context& context)
:   Drawable(context),
    v_frames_(1),
    h_frames_(1),
    frame_(0),
    vertices_(nullptr),
    indices_(nullptr),
    texture_(nullptr)
{
    auto r = context_.get_system<Renderer>();
    vertices_ = r->create_vertex_buffer(sizeof(float) * 8, 4);
    indices_ = r->create_index_buffer(6);
}

Sprite::~Sprite()
{
    delete vertices_;
    delete indices_;
}

void Sprite::draw(Renderer* renderer) const
{
    if (owning_entity_ != nullptr) {
        renderer->set_model_transform(owning_entity_->get_global_transform());
    }
    renderer->set_draw_mode(Renderer::DrawMode::Triangles);
    renderer->draw_indexed(*vertices_, *indices_, texture_, nullptr);
}

void Sprite::set_texture(const Texture* tex)
{
    if (tex == nullptr) {
        return;
    }

    texture_ = tex;

    if (rect_.x == 0 && rect_.y == 0 && rect_.w == 0 && rect_.h == 0) {
        set_rect({texture_->get_size().x, texture_->get_size().y});
    }

    if (v_frames_ != 0 || h_frames_ != 0) {
        set_frame(frame_);
    }

    update_geometry_();
}

void Sprite::set_frames(uint h_frames, uint v_frames)
{
    if (h_frames == 0 || v_frames == 0) {
        sun_log_warn("Sprite::set_frames: (v_frame || h_frame) > 0");
        return;
    }

    h_frames_ = h_frames;
    v_frames_ = v_frames;

    set_frame(0);

    update_geometry_();
}

void Sprite::set_frame(uint frame)
{
    if (frame > v_frames_ * h_frames_) {
        sun_log_warn("Sprite::set_frame: frame > v_frames * h_frames, set to 0");
        frame_ = 0;
        return;
    }
    frame_ = frame;

    if (texture_ != nullptr)
    {
        Vector2u frame_size(texture_->get_size().x / h_frames_,
                            texture_->get_size().y / v_frames_);

        set_rect({(frame % h_frames_) * frame_size.x,
                  (frame / h_frames_) * frame_size.y,
                  frame_size.x, frame_size.y});
    } else {
        sun_log_warn("Sprite::set_frame: sprite has no texture set");
    }
}

void Sprite::update_geometry_()
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
            ((float)rect_.x + (float)rect_.w) / (float)texture_->get_size().w,
            (float)rect_.y / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            (float)rect_.w,
            (float)rect_.h,
            ((float)rect_.x + (float)rect_.w) / (float)texture_->get_size().w,
            ((float)rect_.y + (float)rect_.h) / (float)texture_->get_size().h,
            1.f, 1.f, 1.f, 1.f
        },
        {
            0.f,
            (float)rect_.h,
            (float)rect_.x / (float)texture_->get_size().w,
            ((float)rect_.y + (float)rect_.h) / (float)texture_->get_size().h,
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

void Sprite::build_properties()
{
    properties_.push_back(std::hash<std::string>{}("frame"));
}

void Sprite::set_property_(size_t property_idx, Variant var)
{
    switch(property_idx)
    {
        case 0:
            set_frame(std::get<int>(var));
            break;
    }
}

}
