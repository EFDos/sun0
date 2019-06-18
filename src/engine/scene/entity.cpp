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
#include "system/component.hpp"
#include "math/math.hpp"

namespace sun {

Entity::Entity(Context& context)
:   Object(context),
    id_(0),
    transform_mask_((uint8)transform_bits::translation |
                    (uint8)transform_bits::rotation |
                    (uint8)transform_bits::scale),
    parent_(nullptr),
    scale_(1.f, 1.f),
    rot_(0.f),
    z_order_(0.f),
    dirty_(false)
{
}

Entity* Entity::create_child()
{
    Entity* child = new Entity(context_);
    children_.push_back(child);
    child->parent_ = this;
    return child;
}

Entity::~Entity()
{
    for (auto child : children_) {
        delete child;
    }
}

void Entity::move(float x, float y)
{
    set_position(pos_.x + x, pos_.y + y);
}

void Entity::move(const Vector2f& pos)
{
    set_position(pos_ + pos);
}

void Entity::scale(float x, float y)
{
    set_scale(scale_.x + x, scale_.y + y);
}

void Entity::scale(const Vector2f& scale)
{
    set_scale(scale_ + scale_);
}

void Entity::rotate(float angle)
{
    set_rotation(rot_ + angle);
}

void Entity::set_position(float x, float y)
{
    pos_ = {x, y};
    mark_dirty_();
}

void Entity::set_position(const Vector2f& pos)
{
    pos_ = pos;
    mark_dirty_();
}

void Entity::set_scale(float x, float y)
{
    scale_ = {x, y};
    mark_dirty_();
}

void Entity::set_scale(const Vector2f& scale)
{
    scale_ = scale;
    mark_dirty_();
}

void Entity::set_origin(float x, float y)
{
    origin_ = {x, y};
    mark_dirty_();
}

void Entity::set_origin(const Vector2f& origin)
{
    origin_ = origin;
    mark_dirty_();
}

void Entity::set_rotation(float angle)
{
    rot_ = std::fmod(angle, 360);

    if(rot_ < 0){
        rot_ += 360.f;
    }

    mark_dirty_();
}

void Entity::set_z_order(float z)
{
    z_order_ = z;
}

void Entity::set_global_transform(const Matrix4& transform)
{
    global_transform_ = transform;
    dirty_ = false;
}

void Entity::set_transform_mask(uint8 transform_bits)
{
    transform_mask_ = transform_bits;
}

void Entity::set_translation_bit(bool bit)
{
    if (bit) {
        transform_mask_ |= (uint8)transform_bits::translation;
    } else {
        transform_mask_ &= ~(uint8)transform_bits::translation;
    }
}

void Entity::set_rotation_bit(bool bit)
{
    if (bit) {
        transform_mask_ |= (uint8)transform_bits::rotation;
    } else {
        transform_mask_ &= ~(uint8)transform_bits::rotation;
    }
}

void Entity::set_scale_bit(bool bit)
{
    if (bit) {
        transform_mask_ |= (uint8)transform_bits::scale;
    } else {
        transform_mask_ &= ~(uint8)transform_bits::scale;
    }
}

bool Entity::is_dirty() const
{
    return dirty_;
}

void Entity::clear_dirty_flag()
{
    dirty_ = false;
}

const Vector2f& Entity::get_position() const
{
    return pos_;
}

const Vector2f& Entity::get_scale() const
{
    return scale_;
}

const Vector2f& Entity::get_origin() const
{
    return origin_;
}

float Entity::get_rotation() const
{
    return rot_;
}

float Entity::get_z_order() const
{
    return z_order_;
}

Matrix4 Entity::get_local_transform() const
{
    Matrix4 transform;
    float angle     = -rot_ * math::PI / 180.f;
    float cosine    = std::cos(angle);
    float sine      = std::sin(angle);
    float sxc       = scale_.x * cosine;
    float syc       = scale_.y * cosine;
    float sxs       = scale_.x * sine;
    float sys       = scale_.y * sine;
    float tx        = -origin_.x * sxc - origin_.y * sys + pos_.x;
    float ty        =  origin_.x * sxs - origin_.y * syc + pos_.y;

    transform.set_transformation(sxc, sys, tx, -sxs, syc, ty, 0.f, 0.f, 1.f);

    return transform;
}

const Matrix4& Entity::get_global_transform() const
{
    if (dirty_) {
        if (parent_ != nullptr) {

            // All bits are true, just multiply the matrices
            if (transform_mask_ == 7) {
                global_transform_ = parent_->get_global_transform() *
                get_local_transform();
            } else {
                // Ignore false transform bits
                Matrix4 parent_transform = parent_->get_global_transform();
                if (!(transform_mask_ & (uint8)transform_bits::translation)) {
                    parent_transform.set_translation(0.f, 0.f);
                }
                if (!(transform_mask_ & (uint8)transform_bits::rotation)) {
                    parent_transform.set_rotation(0.f);
                }
                if (!(transform_mask_ & (uint8)transform_bits::scale)) {
                    parent_transform.set_scale(1.f, 1.f);
                }
                global_transform_ = parent_transform * get_local_transform();
            }
        } else {
            global_transform_ = get_local_transform();
        }
        dirty_ = false;
    }
    return global_transform_;
}

const Matrix4& Entity::get_inverse_transform() const
{
    if (dirty_) {
        inverse_transform_ = get_global_transform().get_inverse();
    }
    return inverse_transform_;
}

Component* Entity::get_component_(uint id)
{
    Component* comp = nullptr;
    for (auto c : components_) {
        if (c->get_id() == id) {
            comp = c;
        }
    }
    return comp;
}

void Entity::mark_dirty_()
{
    dirty_ = true;
    for (auto child : children_) {
        child->mark_dirty_();
    }
}

}
