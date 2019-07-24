/*************************************************************************/
/*  animation.hpp                                                        */
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

#include "common/variant.hpp"
#include "system/component.hpp"

#include <vector>
#include <string>

namespace sun {

class Animatable;

enum class AnimationCurve
{
    Linear,
    Cubic
};

class AnimationTrack
{
public:

    struct KeyFrame
    {
        float       position;
        Variant     value;

        template<typename T>
        KeyFrame(T p_value, float p_position = 0.f)
        :   position(p_position), value(p_value)
        {}
    };

    AnimationTrack(Animatable& target, uint property = 0,
        AnimationCurve curve = AnimationCurve::Linear,
        float length = 1.f);

    void update(float delta);

    inline void insert_key(KeyFrame&& key) {
        track_.emplace_back(key);
    }

    inline void set_curve(AnimationCurve curve) {
        curve_ = curve;
    }

    inline AnimationCurve get_curve() const {
        return curve_;
    }

private:

    Animatable&             target_;
    uint                    property_;

    float   length_;
    float   track_pos_;
    size_t  current_keyframe_;

    AnimationCurve          curve_;
    std::vector<KeyFrame>   track_;
};

class Animation : public Component
{
public:

    SUN_COMPONENT_TYPE(Animation)

    Animation(Context&);

    void create_track(Animatable& target, const std::string& property, AnimationCurve curve, Time duration);

private:

    std::vector<AnimationTrack> tracks_;
};

}
