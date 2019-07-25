/*************************************************************************/
/*  audio_server.cpp                                                     */
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
#include "audio_server.hpp"

#include "core/logger.hpp"
#include "sound_stream.hpp"

namespace sun {

AudioServer::AudioServer(Context& context)
:   System(context),
    volume_(0.f),
    alc_device_(nullptr),
    alc_capture_device_(nullptr),
    alc_context_(nullptr)
{
}

bool AudioServer::init()
{
    alGetError();
    alc_device_ = alcOpenDevice(0);

    if (alc_device_ == nullptr) {
        sun_log_error("OpenAL failed to open default audio device.");
        return false;
    } else {
        ALCenum error = alcGetError(alc_device_);

        if (error != ALC_NO_ERROR)
        {
            alcCloseDevice(alc_device_);
            sun_logf_error("OpenAL failed opening audio device: %s",
                    get_alc_error(error));
            return false;
        }
    }

    alc_context_ = alcCreateContext(alc_device_, 0);

    if (alc_context_ == nullptr) {
        sun_log_error("OpenAL failed to create audio context");
        return false;
    } else {
        ALCenum error = alcGetError(alc_device_);

        if (error != ALC_NO_ERROR)
        {
            alcDestroyContext(alc_context_);
            alcCloseDevice(alc_device_);

            sun_logf_error("OpenAL failed to create audio context: %s",
                get_alc_error(error));
            return false;
        }
    }

    if (alcMakeContextCurrent(alc_context_) != ALC_TRUE) {
        alcDestroyContext(alc_context_);
        alcCloseDevice(alc_device_);
        sun_log_error("OpenAL failed to make context current.");
        return false;
    }

    set_global_volume(50.f);
    set_listener({
        {0.f, 0.f, 0.f},    // position
        {0.f, 0.f, 0.f},    // velocity
        {0.f, 0.f, -1.f},   // orientation
        {0.f, 1.f, 0.f}     // up vector
    });

    alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED);

    sun_log_info("OpenAL Audio Server initialized.");
    return System::init();
}

void AudioServer::shutdown()
{
    if (alc_context_ != nullptr) {
        alcSuspendContext(alc_context_);
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(alc_context_);
    }

    if (alc_device_ != nullptr) {
        alcCloseDevice(alc_device_);
    }

    if (alc_capture_device_ != nullptr) {
        alcCloseDevice(alc_capture_device_);
    }

    for (auto snd_src : sound_sources_) {
        delete snd_src;
    }

    sound_sources_.clear();

    sun_log_info("OpenAL Audio Server shutdown.");
    System::shutdown();
}

void AudioServer::set_global_volume(float volume)
{
    volume_ = volume;
    alListenerf(AL_GAIN, volume * 0.01f);
}

void AudioServer::set_listener(const AudioListener3D& listener)
{
    set_listener_position(listener.position);
    set_listener_velocity(listener.velocity);
    set_listener_orientation(listener.orientation, listener.up_vector);
}

void AudioServer::set_listener_position(Vector3f pos)
{
    listener_.position = pos;
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioServer::set_listener_velocity(Vector3f vel)
{
    listener_.velocity = vel;
    alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioServer::set_listener_orientation(Vector3f ori, Vector3f up_vec)
{
    listener_.orientation = ori;
    listener_.up_vector = up_vec;

    ALfloat orientation[6];

    orientation[0] = ori.x;
    orientation[1] = ori.y;
    orientation[2] = ori.z;
    orientation[3] = up_vec.x;
    orientation[4] = up_vec.y;
    orientation[5] = up_vec.z;

    alListenerfv(AL_ORIENTATION, orientation);
}

int AudioServer::get_format_from_channel_count(uint count) const
{
    if(alc_device_ == nullptr) {
        sun_log_warn("Querying format while AudioServer is uinitialized");
    }

    int format = 0;

    switch(count)
    {
        case 1: format = AL_FORMAT_MONO16; break;
        case 2: format = AL_FORMAT_STEREO16; break;
        case 4: format = alGetEnumValue("AL_FORMAT_QUAD16"); break;
        case 6: format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
        case 7: format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
        case 8: format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
    }

    if(format == -1)
        format = 0;

    return format;
}

const char* AudioServer::get_al_error(ALenum error) const noexcept
{
    switch (error)
    {
        case AL_INVALID_NAME:
            return "AL: Invalid name";
        case AL_INVALID_ENUM:
            return "AL: Invalid enum";
        case AL_INVALID_VALUE:
            return "AL: Invalid value";
        case AL_INVALID_OPERATION:
            return "AL: Invalid operation";
        case AL_OUT_OF_MEMORY:
            return "AL: Out of memory";
        default:
            return std::string("AL Error code: " + std::to_string(error))
                .c_str();
    }
}

const char* AudioServer::get_alc_error(ALCenum error) const noexcept
{
    switch (error)
    {
        case ALC_INVALID_ENUM:
            return "ALC: Invalid enum";
        case ALC_INVALID_VALUE:
            return "ALC: Invalid value";
        case ALC_INVALID_DEVICE:
            return "ALC: Invalid device";
        case ALC_INVALID_CONTEXT:
            return "ALC: Invalid context";
        case ALC_OUT_OF_MEMORY:
            return "ALC: Out of memory";
        default:
            return std::string("ALC Error code: " + std::to_string(error))
                .c_str();
    }
}

Component* AudioServer::create_component_(uint type_hash, uint id)
{
    Component* comp = nullptr;
    if (type_hash == SoundStream::get_static_type_hash()) {
        comp = new SoundStream(context_);
        sound_sources_.push_back(static_cast<SoundStream*>(comp));
    }
    comp->set_id(id);
    return comp;
}

bool AudioServer::handles_component_(uint type_hash)
{
    if (type_hash == SoundStream::get_static_type_hash()) {
        return true;
    }
    return false;
}

}
