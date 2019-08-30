/*************************************************************************/
/*  drawable.hpp                                                         */
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

#include "system/component.hpp"

namespace sun {

class Renderer;
class Matrix4;
class Shader;

class SUN_API Drawable : public Component
{
public:

    Drawable(Context& context)
    :   Component(context), dirty_(true), transform_(nullptr), shader_(nullptr)
    {
        set_handle_event(false);
        set_update(false);
    }

    virtual ~Drawable() {}

    virtual void draw(Renderer*) const = 0;

    void prepare_draw() {
        if (dirty_) {
            update_geometry_();
            dirty_ = false;
        }
    }

    /*recti get_global_bounding_rect() const {
        recti rect(bounding_rect_);
        if (owning_entity_ != nullptr) {
            rect.set_point((int)owning_entity_->get_position().x, (int)owning_entity_->get_position().y);
        }
        return rect;
    }*/

    inline void set_transform(const Matrix4* transform) {
        transform_ = transform;
    }

    inline void set_shader(Shader* shader) {
        shader_ = shader;
    }

    const Recti& get_bounding_rect() const {
        return bounding_rect_;
    }

protected:

    virtual void update_geometry_() = 0;

    bool dirty_;

    Recti   bounding_rect_;

    const Matrix4*    transform_;
    Shader*     shader_;
};

}
