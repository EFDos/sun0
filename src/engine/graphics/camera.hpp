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

class renderer;

class SUN_API camera final : public component
{
public:

    SUN_COMPONENT_TYPE(camera)

    camera(context&);

    void update_transform(renderer&);

    inline void set_viewport_size(const vector2f& size) {
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

    inline void set_follow_offset(const recti& offset) {
        offset_ = offset;
    }

    inline void set_limits(const recti& limits) {
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

    inline const recti& get_follow_offset() const {
        return offset_;
    }

    inline const recti& get_limits() const {
        return limit_;
    }

    vector2f get_center() const;

private:

    rectf   viewport_;
    recti   offset_;
    recti   limit_;

    float   follow_speed_;

    bool    follow_;
    bool    hard_limit_;

    matrix4 transform_;
};

} // sun
