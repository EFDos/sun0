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

#include "common/object.hpp"
#include "math/vector3.hpp"

namespace sun {

class context;

class SUN_API sound_source : public object
{
public:

    enum class state {
        stopped,
        paused,
        playing
    };

    virtual ~sound_source();

    virtual void play() = 0;

    virtual void pause() = 0;

    virtual void stop() = 0;

    void set_volume(float vol);

    void set_pitch(float pitch);

    void set_relative(bool relative);

    void set_minmum_distance(float dist);

    void set_attenuation(float atten);

    void set_position(float x, float y, float z);

    void set_position(const vector3f& pos);

    float get_volume() const;

    float get_pitch() const;

    bool is_relative() const;

    float get_minimum_distance() const;

    float get_attenuation() const;

    virtual state get_state() const;

    vector3f get_position() const;

protected:

    sound_source(context& p_context);

    sound_source(const sound_source&);

    sound_source(sound_source&&) = default;

    uint source_;
};

}
