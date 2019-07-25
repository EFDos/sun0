/*************************************************************************/
/*  light2D.cpp                                                          */
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
#include "light2D.hpp"

#include "core/context.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"

namespace sun {

Light2D::Light2D(Context& context)
:   Component(context),
    rend_color_id_(-1),
    intensity_(0.f),
    color_(sun::Color::SUN)
{}

void Light2D::update(float delta)
{
    if (owning_entity_ == nullptr) {
        return;
    }

    if (owning_entity_->get_position() != pos_) {
        pos_ = owning_entity_->get_position();
        update_position_();
    }
}

void Light2D::update_intensity_()
{
    auto renderer = context_.get_system<Renderer>();

    if (rend_color_id_ == -1) {
        if (owning_entity_ != nullptr) {
            pos_ = owning_entity_->get_position();
        }
        rend_color_id_ = renderer->add_light(pos_,
            Color::to_colorf(color_),
            intensity_);
    } else {
        renderer->update_light(rend_color_id_, intensity_);
    }
}

void Light2D::update_color_()
{
    auto renderer = context_.get_system<Renderer>();

    if (rend_color_id_ == -1) {
        if (owning_entity_ != nullptr) {
            pos_ = owning_entity_->get_position();
        }
        rend_color_id_ = renderer->add_light(pos_,
            Color::to_colorf(color_),
            intensity_);
    } else {
        renderer->update_light(rend_color_id_, Color::to_colorf(color_));
    }
}

void Light2D::update_position_()
{
    auto renderer = context_.get_system<Renderer>();

    if (rend_color_id_ == -1) {
        rend_color_id_ = renderer->add_light(pos_,
            Color::to_colorf(color_),
            intensity_);
    } else {
        renderer->update_light(rend_color_id_, pos_);
    }
}

}
