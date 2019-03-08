/*************************************************************************/
/*  sound_source.hpp                                                     */
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

#include "common/config.hpp"
#include "math/vector3.hpp"

namespace sun {

class SUN_API sound_source
{
public:

    enum class status {
        stopped,
        paused,
        playing
    };

    sound_source();

    virtual ~sound_source();

    virtual void play();

    virtual void pause();

    virtual void stop();

    inline virtual void set_volume(float vol) {
        volume_ = vol;
    }

    inline virtual void set_pitch(float pitch) {
        pitch_ = pitch;
    }

    inline virtual void set_relative(bool relative) {
        relative_ = relative;
    }

    inline virtual void set_minmum_distance(float dist) {
        min_distance_ = dist;
    }

    inline virtual void set_attenuation(float atten) {
        attenuation_ = atten;
    }

    inline virtual void set_position(float x, float y, float z) {
        position_ = {x, y, z};
    }

    virtual void set_position(const vector3f& pos) {
        position_ = pos;
    }

    float get_volume() const { return volume_; }

    float get_pitch() const { return pitch_; }

    bool is_relative() const { return relative_; }

    float get_minimum_distance() const { return min_distance_; }

    float get_attenuation() const { return attenuation_; }

    const vector3f& get_position() const { return position_; }

private:

    float       volume_;
    float       pitch_;
    float       min_distance_;
    float       attenuation_;

    vector3f    position_;

    bool        relative_;
    status      status_;
};

}
