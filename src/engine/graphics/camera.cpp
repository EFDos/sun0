/*************************************************************************/
/*  camera.cpp                                                           */
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
#include "camera.hpp"
#include "renderer.hpp"
#include "scene/entity.hpp"
#include "core/logger.hpp"

namespace sun {

Camera::Camera(Context& context)
:   Component(context),
    renderer_(nullptr),
    viewport_({0.f, 0.f, 0.f, 0.f}),
    offset_({280.f, 280.f, 280.f, 280.f}),
    follow_speed_(20.f),
    zoom_factor_(1.f),
    follow_(false),
    hard_limit_(false)
{
}

void Camera::init()
{
    renderer_ = context_.get_system<Renderer>();
    viewport_.set_size(renderer_->get_viewport().get_size());

    Component::init();
}

void Camera::update(float delta)
{
    if (owning_entity_ != nullptr && follow_)
    {
        const auto& parent_pos = owning_entity_->get_position();
        float distance = 0.f;

        if (parent_pos.x > (viewport_.x + (viewport_.w - offset_.w))) {
            distance = parent_pos.x - (viewport_.x + (viewport_.w - offset_.w));

            if (!hard_limit_)
            {
                viewport_.x += distance * follow_speed_ * delta;
            }
        }
        else if (parent_pos.x < (viewport_.x + offset_.x))
        {
            distance = viewport_.x + offset_.x - parent_pos.x;

            if (!hard_limit_)
            {
                viewport_.x -= distance * follow_speed_ * delta;
            }
        }

        if (parent_pos.y > (viewport_.y + (viewport_.h - offset_.h)))
        {
            distance = parent_pos.y - (viewport_.y +
                                       (viewport_.h - offset_.w));

            if (!hard_limit_)
            {
                viewport_.y += distance * follow_speed_ * delta;
            }
        }
        else if (parent_pos.y < (viewport_.y + offset_.y))
        {
            distance = viewport_.y + offset_.y - parent_pos.y;

            if (!hard_limit_)
            {
                viewport_.y -= distance * follow_speed_ * delta;
            }
        }

        transform_.set_translation(-viewport_.get_position());
        transform_.set_scale(zoom_factor_, zoom_factor_);
        if (renderer_ != nullptr) {
            renderer_->set_camera_transform(transform_);
        }
    }
}


Vector2f Camera::get_center() const
{
    return {viewport_.x + viewport_.w / 2.f,
            viewport_.y + viewport_.h / 2.f};
}

}
