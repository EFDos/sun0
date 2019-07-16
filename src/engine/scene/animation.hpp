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

#include "common/types.hpp"
#include "system/component.hpp"

#include <vector>
#include <variant>
#include <string>

namespace sun {

enum class AnimationCurve
{
    Linear,
    Cubic
};

class AnimationTrack
{
public:

    enum class DataType
    {
        Int = 0,
        Float,
        Vector2f
    };

    using VariantData = std::variant<int, float, Vector2f>;

    struct KeyFrame
    {
        float       position;
        VariantData value;
        DataType    type;

        template<typename T>
        KeyFrame(T p_value, DataType p_type, float p_position = 0.f)
        :   position(p_position), value(p_value), type(p_type)
        {}
    };

    AnimationTrack(uint property, AnimationCurve curve = AnimationCurve::Linear, float length = 1.f)
    :   length_(length), property_(property), curve_(curve)
    {}

    void insert_key(KeyFrame&& key) {
        track_.emplace_back(key);
    }

    inline void set_curve(AnimationCurve curve) {
        curve_ = curve;
    }

    inline AnimationCurve get_curve() const {
        return curve_;
    }

private:

    float length_;

    std::vector<KeyFrame>   track_;
    uint                    property_;
    AnimationCurve          curve_;
};

class Animation : public Component
{
public:

    SUN_COMPONENT_TYPE(Animation)

    Animation(Context&);

    void create_track(const std::string& property, AnimationCurve curve, Time duration);

private:

    std::vector<AnimationTrack> tracks_;
};

}
