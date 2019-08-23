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
#pragma once

#include "animatable.hpp"
#include "common/object.hpp"

#include "core/context.hpp"

namespace sun {

class SceneTree;
class Component;

class SUN_API Entity : Object, public Animatable
{
public:

    Entity(Context& context);

    ~Entity();

    template<typename T>
    T* create_component(const std::string& name = "") {
        auto comp = context_
            .create_component<T>(std::hash<std::string>{}(name), false);
        if (comp != nullptr) {
            comp->set_owning_entity(this);
            components_.push_back(comp);
            return static_cast<T*>(comp);
        }
        return nullptr;
    }

    template<typename T>
    T* get_component(const std::string& name) {
        T* comp = nullptr;
        uint hash = std::hash<std::string>{}(name);
        comp = static_cast<T*>(get_component_(hash));
        return comp;
    }

    void init_components(bool recursive = false);

    void build_properties() override;

    Entity* create_child(const std::string& name = "");

    Entity* find_child(const std::string& name, bool recursive = false) const;

    Entity* find_child(uint64 id, bool recursive = false) const;

    void clear_children();

    void clear_components();

    void move(float x, float y);

    void move(const Vector2f& pos);

    void scale(float x, float y);

    void scale(const Vector2f& scale);

    void rotate(float angle);

    void set_position(float x, float y);

    void set_position(const Vector2f& pos);

    void set_scale(float x, float y);

    void set_scale(const Vector2f& scale);

    void set_origin(float x, float);

    void set_origin(const Vector2f&);

    void set_rotation(float angle);

    void set_z_order(float z);

    void set_local_transform(const Matrix4&);

    void set_global_transform(const Matrix4&);

    void set_transform_mask(uint8 transform_bits);

    void set_translation_bit(bool);

    void set_rotation_bit(bool);

    void set_scale_bit(bool);

    inline void set_name(const std::string& name) {
        name_ = name;
    }

    inline void set_scene(SceneTree* scene) {
        scene_ = scene;
    }

    void clear_dirty_flag();

    bool is_dirty() const;

    Entity* get_child(const std::string& path) const;

    const Vector2f& get_position() const;

    const Vector2f& get_scale() const;

    const Vector2f& get_origin() const;

    float get_rotation() const;

    float get_z_order() const;

    Matrix4 get_local_transform() const;

    const Matrix4& get_global_transform() const;

    const Matrix4& get_inverse_transform() const;

    inline uint64 get_id() const {
        return id_;
    }

    inline const std::string& get_name() const {
        return name_;
    }

    inline size_t get_children_count() const {
        return children_.size();
    }

    inline size_t get_component_count() const {
        return components_.size();
    }

private:

    void set_property_(size_t property_idx, Variant var) override;

    Component* get_component_(uint id);

    enum class transform_bits : uint8
    {
        translation = 1,
        rotation    = 2,
        scale       = 4
    };

    void mark_dirty_();

    uint64  id_;
    uint8   transform_mask_;

    std::string name_;

    SceneTree*              scene_;
    Entity*                 parent_;

    std::vector<Entity*>     children_;
    std::vector<Component*> components_;

    Vector2f    pos_;
    Vector2f    scale_;
    Vector2f    origin_;
    float       rot_;
    float       z_order_;

    mutable bool                dirty_;
    mutable Matrix4     global_transform_;
    mutable Matrix4     inverse_transform_;
};

}
