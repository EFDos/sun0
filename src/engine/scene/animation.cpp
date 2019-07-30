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

AnimationTrack::AnimationTrack(Animatable& target, uint property,
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
            auto val = track_[current_keyframe_].value;
            switch (variant::get_type(val))
            {
                case VariantType::Vector2f:
                    target_.set_property(property_, val);
                    break;
                default:
                    break;
            }
            return;
        }
    }

    KeyFrame& curr_kf = track_[current_keyframe_];
    KeyFrame& next_kf = track_[current_keyframe_ + 1];

    float length = next_kf.position - curr_kf.position;
    float ratio = (track_pos_  - curr_kf.position) / length;

    if (variant::get_type(curr_kf.value) == VariantType::Vector2f) {
        auto curr_val = std::get<Vector2f>(curr_kf.value);
        auto next_val = std::get<Vector2f>(next_kf.value);

        auto value = curr_val + (next_val - curr_val) * ratio;
        target_.set_property(property_, value);
    }
}

Animation::Animation(Context& context)
:   Component(context)
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
