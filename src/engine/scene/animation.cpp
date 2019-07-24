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
    track_pos_ += delta;

    if (current_keyframe_ == track_.size() - 1) {
        return;
    }

    if (track_pos_ > track_[current_keyframe_ + 1].position) {
        ++current_keyframe_;
    }

    //float diff = track_[current_keyframe_ + 1].position - track_pos_;

    target_.set_property(property_, track_[current_keyframe_ + 1].value);
}

Animation::Animation(Context& context)
:   Component(context)
{}

void Animation::create_track(Animatable& target, const std::string& property,
                             AnimationCurve curve, Time duration)
{
    tracks_.emplace_back(AnimationTrack(target,
        std::hash<std::string>{}(property),
        curve,
        duration.as_seconds()));
}

}
