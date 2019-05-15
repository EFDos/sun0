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
#include "common/object.hpp"
#include "common/types.hpp"

#include "core/context.hpp"

#include <vector>

namespace sun {

class component;

class SUN_API entity : object
{
public:

    entity(context& p_context);

    template<typename T>
    T* create_component() {
        auto comp = context_.create_component<T>();
        if (comp != nullptr) {
            comp->set_owning_entity(this);
            components_.push_back(comp);
            return static_cast<T*>(comp);
        }
        return nullptr;
    }

    void move(float x, float y);

    void move(const vector2f& pos);

    void scale(float x, float y);

    void scale(const vector2f& scale);

    void rotate(float angle);

    void set_position(float x, float y);

    void set_position(const vector2f& pos);

    void set_scale(float x, float y);

    void set_scale(const vector2f& scale);

    void set_origin(float x, float);

    void set_origin(const vector2f&);

    void set_rotation(float angle);

    void set_z_order(float z);

    void set_transform(const matrix4&);

    void clear_dirty_flag();

    bool is_dirty() const;

    const vector2f& get_position() const;

    const vector2f& get_scale() const;

    const vector2f& get_origin() const;

    float get_rotation() const;

    float get_z_order() const;

    const matrix4& get_transform() const;

    const matrix4& get_inverse_transform() const;

private:

    uint64  id_;

    std::vector<entity>     children_;
    std::vector<component*> components_;

    vector2f    pos_;
    vector2f    scale_;
    vector2f    origin_;
    float       rot_;
    float       z_order_;

    bool                dirty_;
    mutable matrix4     transform_;
    mutable bool        transform_update_;
    mutable matrix4     inverse_transform_;
    mutable bool        inverse_transform_update_;
};

}