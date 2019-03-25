/*************************************************************************/
/*  sound_source.cpp                                                     */
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
#include "sound_source.hpp"

#include <AL/al.h>
#include <AL/alc.h>

namespace sun {

sound_source::sound_source(context& p_context)
:   object(p_context),
    source_(0)
{
    alGenSources(1, &source_);
    alSourcei(source_, AL_BUFFER, 0);
}

sound_source::sound_source(const sound_source& other)
:   object(other.context_)
{
    alGenSources(1, &source_);
    alSourcei(source_, AL_BUFFER, 0);

    set_pitch(other.get_pitch());
    set_volume(other.get_volume());
    set_position(other.get_position());
    set_relative(other.is_relative());
    set_minmum_distance(other.get_minimum_distance());
    set_attenuation(other.get_attenuation());
}

sound_source::~sound_source()
{
    alSourcei(source_, AL_BUFFER, 0);
    alDeleteSources(1, &source_);
}

void sound_source::set_pitch(float pitch)
{
    alSourcef(source_, AL_PITCH, pitch);
}

void sound_source::set_volume(float vol)
{
    alSourcef(source_, AL_GAIN, vol * 0.01f);
}

void sound_source::set_position(float x, float y, float z)
{
    alSource3f(source_, AL_POSITION, x, y, z);
}

void sound_source::set_position(const vector3f& pos)
{
    set_position(pos.x, pos.y, pos.z);
}

void sound_source::set_relative(bool relative)
{
    alSourcei(source_, AL_SOURCE_RELATIVE, relative);
}

void sound_source::set_minmum_distance(float distance)
{
    alSourcef(source_, AL_REFERENCE_DISTANCE, distance);
}

void sound_source::set_attenuation(float attenuation)
{
    alSourcef(source_, AL_ROLLOFF_FACTOR, attenuation);
}

float sound_source::get_pitch() const
{
    ALfloat pitch;
    alGetSourcef(source_, AL_PITCH, &pitch);
    return pitch;
}

float sound_source::get_volume() const
{
    ALfloat gain;
    alGetSourcef(source_, AL_GAIN, &gain);
    return gain;
}

vector3f sound_source::get_position() const
{
    vector3f pos;
    alGetSource3f(source_, AL_POSITION, &pos.x, &pos.y, &pos.z);
    return pos;
}

bool sound_source::is_relative() const
{
    ALint relative;
    alGetSourcei(source_, AL_SOURCE_RELATIVE, &relative);
    return relative != 0;
}

float sound_source::get_minimum_distance() const
{
    ALfloat distance;
    alGetSourcef(source_, AL_REFERENCE_DISTANCE, &distance);
    return distance;
}

float sound_source::get_attenuation() const
{
    ALfloat attenuation;
    alGetSourcef(source_, AL_ROLLOFF_FACTOR, &attenuation);
    return attenuation;
}

sound_source::state sound_source::get_state() const
{
    ALint state;
    alGetSourcei(source_, AL_SOURCE_STATE, &state);

    switch(state)
    {
        case AL_INITIAL:
        case AL_STOPPED: return state::stopped;
        case AL_PAUSED: return state::paused;
        case AL_PLAYING: return state::playing;
    }

    return state::stopped;
}

}
