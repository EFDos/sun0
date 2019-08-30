/*************************************************************************/
/*  animation.cpp                                                        */
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
#include "animation.hpp"
#include "animatable.hpp"

namespace sun {

AnimationTrack::AnimationTrack(Animatable& target, uint64 property,
                               AnimationCurve curve, float length)
:   target_(target),
    property_(property),
    length_(length),
    track_pos_(0.f),
    current_keyframe_(0),
    curve_(curve)
{}

void AnimationTrack::update(float delta)
{
    if (track_pos_ >= length_) {
        return;
    }

    track_pos_ += delta;

    if (current_keyframe_ == track_.size()) {
        return;
    }

    if (track_pos_ > track_[current_keyframe_ + 1].position) {
        if (++current_keyframe_ == track_.size() - 1) {
            target_.set_property(property_, track_[current_keyframe_].value);
            return;
        }
    }

    KeyFrame& curr_kf = track_[current_keyframe_];
    KeyFrame& next_kf = track_[current_keyframe_ + 1];

    float length = next_kf.position - curr_kf.position;
    float ratio = (track_pos_  - curr_kf.position) / length;

    switch(variant::get_type(curr_kf.value))
    {
        case VariantType::Vector2f:
            {
                auto curr_val = std::get<Vector2f>(curr_kf.value);
                auto next_val = std::get<Vector2f>(next_kf.value);
                auto value = curr_val + (next_val - curr_val) * ratio;
                target_.set_property(property_, value);
            } break;
        case VariantType::Color:
            {
                auto curr_val = Color::to_colorf(std::get<Color>(curr_kf.value));
                auto next_val = Color::to_colorf(std::get<Color>(next_kf.value));
                auto value = curr_val + (next_val - curr_val) * ratio;
                //value *= ratio;
                target_.set_property(property_, Colorf::to_color(value));
            } break;
        case VariantType::Int:
            {
                int curr_val = std::get<int>(curr_kf.value);
                int next_val = std::get<int>(next_kf.value);
                int value = curr_val + (next_val - curr_val) * ratio;
                target_.set_property(property_, value);
            } break;
        default:
            break;
    }
}

Animation::Animation(Context& context)
:   Component(context)
{
    set_handle_event(false);
    set_draw(false);
}

void Animation::init()
{}

void Animation::update(float delta)
{
    for (auto& track : tracks_) {
        track.update(delta);
    }
}

AnimationTrack& Animation::create_track(Animatable& target, const std::string& property,
                             Time duration, AnimationCurve curve)
{
    target.build_properties();
    tracks_.emplace_back(target,
        std::hash<std::string>{}(property),
        curve,
        duration.as_seconds());

    return tracks_.back();
}

}
