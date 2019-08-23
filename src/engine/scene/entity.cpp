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
#include "scene_tree.hpp"
#include "system/component.hpp"
#include "math/math.hpp"
#include "core/logger.hpp"

#include <algorithm>
#include <sstream>

#include "core/clock.hpp"

namespace sun {

Entity::Entity(Context& context)
:   Object(context),
    id_(0),
    transform_mask_((uint8)transform_bits::translation |
                    (uint8)transform_bits::rotation |
                    (uint8)transform_bits::scale),
    scene_(nullptr),
    parent_(nullptr),
    scale_(1.f, 1.f),
    rot_(0.f),
    z_order_(0.f),
    dirty_(false)
{
}

Entity* Entity::create_child(const std::string& name)
{
    Entity* child = new Entity(context_);
    children_.push_back(child);
    child->scene_ = scene_;
    child->id_ = scene_->get_available_id();
    if (name.empty()) {
        child->name_ = "Entity" + std::to_string(child->id_);
    } else {
        child->name_ = name;
    }
    child->parent_ = this;
    return child;
}

void Entity::clear_children()
{
    for (auto child : children_) {
        delete child;
    }

    children_.clear();
}

void Entity::clear_components()
{
    for (auto component : components_) {
        component->queue_delete();
        component->set_owning_entity(nullptr);
    }

    components_.clear();
}

Entity* Entity::find_child(const std::string& name, bool recursive) const
{
    // First seek on the first layer only
    for (auto child : children_) {
        sun_logf_debug("found child of name: %s", child->name_.c_str());
        if (child->name_ == name) {
            return child;
        }
    }

    // If we get here, it must be deeper. Continue if recursive
    Entity* lost_child = nullptr;
    if (recursive) {
        for (auto child : children_) {
            lost_child = child->find_child(name, true);

            if (lost_child != nullptr) {
                return lost_child;
            }
        }
    }

    // If we get here, it's nowhere to be found. Returns nullptr
    return nullptr;
}

Entity* Entity::find_child(uint64 id, bool recursive) const
{
    // Same as the find_child by name implementation
    for (auto child : children_) {
        if (child->id_ == id) {
            return child;
        }
    }

    Entity* lost_child = nullptr;
    if (recursive) {
        for (auto child : children_) {
            lost_child = child->find_child(id_, true);

            if (lost_child != nullptr) {
                return lost_child;
            }
        }
    }

    return nullptr;
}

Entity::~Entity()
{
    clear_children();
    clear_components();
}

void Entity::init_components(bool recursive)
{
    for (auto component : components_) {
        component->init();
    }

    if (recursive) {
        for (auto child : children_) {
            child->init_components(true);
        }
    }
}

void Entity::build_properties()
{
    properties_.push_back(std::hash<std::string>{}("position"));
}

void Entity::set_property_(size_t property_idx, Variant var)
{
    switch (property_idx)
    {
        case 0:
            set_position(std::get<Vector2f>(var));
            break;
        default:
            return;
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
                // Ignore off transform bits

                // To check if parent's origin is not 0, 0
                const Vector2f parent_origin = parent_->get_origin();
                bool recover_origin = false;

                if (parent_origin != 0) {
                    // We'll temporarily set it to 0, 0 to calculate our
                    // transformation
                    parent_->set_origin(0.f, 0.f);
                    recover_origin = true;
                }

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

                // If we zeroed parent's origin, set it back
                if (recover_origin) {
                    parent_->set_origin(parent_origin);
                }
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

Entity* Entity::get_child(const std::string& path) const
{
    if (path.find('/') == std::string::npos) {
        Entity* lost_child = find_child(path);

        if (lost_child != nullptr) {
            return lost_child;
        }
    }

    // Break path into segments and follow
    std::stringstream pathstream(path);
    std::string segment;
    std::vector<std::string> list;

    list.reserve(std::count(path.begin(), path.end(), '/') + 1);

    while (std::getline(pathstream, segment, '/')) {
        list.push_back(segment);
    }

    Entity* next_child = nullptr;
    for (auto& seg : list) {
        sun_logf_debug("looking for: %s", seg.c_str());
        if (next_child == nullptr) {
            next_child = find_child(seg);
        } else {
            next_child = next_child->find_child(seg);
        }


        if (next_child == nullptr) {
            sun_log_debug("not found");
            return nullptr;
        }
        sun_log_debug("found");
    }

    return next_child;
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
