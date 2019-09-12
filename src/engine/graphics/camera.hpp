/*************************************************************************/
/*  camera.hpp                                                           */
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
#include "common/types.hpp"

namespace sun {

class Renderer;

class SUN_API Camera final : public Component
{
public:

    SUN_COMPONENT_TYPE(Camera)

    Camera(Context&);

    void init() override;

    void update(float delta) override;

    inline void zoom(float zoom_factor) {
        zoom_factor_ += zoom_factor;
    }

    inline void set_viewport_size(const Vector2f& size) {
        viewport_.set_size(size);
    }

    inline void set_viewport_size(float w, float h) {
        viewport_.set_size({w, h});
    }

    inline void set_follow(bool follow) {
        follow_ = follow;
    }

    inline void set_hard_limit(bool hard_limit) {
        hard_limit_ = hard_limit;
    }

    inline void set_follow_speed(float speed) {
        follow_speed_ = speed;
    }

    inline void set_follow_offset(const Rectf& offset) {
        offset_ = offset;
    }

    inline void set_limits(const Recti& limits) {
        limit_ = limits;
    }

    inline bool get_follow() const {
        return follow_;
    }

    inline bool get_hard_limit() const {
        return hard_limit_;
    }

    inline float get_follow_speed() const {
        return follow_speed_;
    }

    inline const Rectf& get_follow_offset() const {
        return offset_;
    }

    inline const Recti& get_limits() const {
        return limit_;
    }

    Vector2f get_center() const;

private:

    Renderer*   renderer_;

    Rectf   viewport_;
    Rectf   offset_;
    Recti   limit_;

    float   follow_speed_;
    float   zoom_factor_;

    bool    follow_;
    bool    hard_limit_;

    Matrix4 transform_;
};

} // sun
