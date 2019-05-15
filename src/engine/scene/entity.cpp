/*************************************************************************/
/*  entity.hpp                                                           */
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
#include "entity.hpp"
#include "math/math.hpp"

namespace sun {

entity::entity(context& p_context)
:   object(p_context),
    id_(0),
    scale_(1.f, 1.f),
    rot_(0.f),
    z_order_(0.f),
    dirty_(false),
    transform_update_(false),
    inverse_transform_update_(false)
{
}

void entity::move(float x, float y)
{
    set_position(pos_.x + x, pos_.y + y);
}

void entity::move(const vector2f& pos)
{
    set_position(pos_ + pos);
}

void entity::scale(float x, float y)
{
    set_scale(scale_.x + x, scale_.y + y);
}

void entity::scale(const vector2f& scale)
{
    set_scale(scale_ + scale_);
}

void entity::rotate(float angle)
{
    set_rotation(rot_ + angle);
}

void entity::set_position(float x, float y)
{
    pos_ = {x, y};
    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_position(const vector2f& pos)
{
    pos_ = pos;
    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_scale(float x, float y)
{
    scale_ = {x, y};
    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_scale(const vector2f& scale)
{
    scale_ = scale;
    dirty_= transform_update_ = inverse_transform_update_ = true;
}

void entity::set_origin(float x, float y)
{
    origin_ = {x, y};
    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_origin(const vector2f& origin)
{
    origin_ = origin;
    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_rotation(float angle)
{
    rot_ = std::fmod(angle, 360);

    if(rot_ < 0){
        rot_ += 360.f;
    }

    dirty_ = transform_update_ = inverse_transform_update_ = true;
}

void entity::set_z_order(float z)
{
    z_order_ = z;
}

void entity::set_transform(const matrix4& transform)
{
    transform_ = transform;
    dirty_ = false;
    transform_update_ = false;
    inverse_transform_update_ = false;
}

bool entity::is_dirty() const
{
    return dirty_;
}

void entity::clear_dirty_flag()
{
    dirty_ = false;
}

const vector2f& entity::get_position() const
{
    return pos_;
}

const vector2f& entity::get_scale() const
{
    return scale_;
}

const vector2f& entity::get_origin() const
{
    return origin_;
}

float entity::get_rotation() const
{
    return rot_;
}

float entity::get_z_order() const
{
    return z_order_;
}

const matrix4& entity::get_transform() const
{
    if (transform_update_)
    {
        float angle     = -rot_ * math::pi / 180.f;
        float cosine    = std::cos(angle);
        float sine      = std::sin(angle);
        float sxc       = scale_.x * cosine;
        float syc       = scale_.y * cosine;
        float sxs       = scale_.x * sine;
        float sys       = scale_.y * sine;
        float tx        = -origin_.x * sxc - origin_.y * sys + pos_.x;
        float ty        =  origin_.x * sxs - origin_.y * syc + pos_.y;

        transform_.set_transformation(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);
        transform_update_ = false;
    }

    return transform_;
}

const matrix4& entity::get_inverse_transform() const
{
    if (inverse_transform_update_)
    {
        inverse_transform_ = get_transform().get_inverse();
        inverse_transform_update_ = false;
    }

    return inverse_transform_;
}

}
