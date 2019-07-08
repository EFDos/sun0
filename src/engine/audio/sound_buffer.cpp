/*************************************************************************/
/*  sound_buffer.cpp                                                     */
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
#include "sound_buffer.hpp"

#include "AL/al.h"

namespace sun {

SoundBuffer::SoundBuffer(Context& context)
:   SoundSource(context),
    al_buffer_(0),
    sample_count_(0),
    duration_(0.f)
{
    alGenBuffers(1, &al_buffer_);
}

SoundBuffer::~SoundBuffer()
{
    alDeleteBuffers(1, &al_buffer_);
}

uint SoundBuffer::get_sample_rate() const
{
    ALint sample_rate;
    alGetBufferi(al_buffer_, AL_FREQUENCY, &sample_rate);
    return sample_rate;
}

uint SoundBuffer::get_channel_count() const
{
    ALint channel_count;
    alGetBufferi(al_buffer_, AL_CHANNELS, &channel_count);
    return channel_count;
}

}
