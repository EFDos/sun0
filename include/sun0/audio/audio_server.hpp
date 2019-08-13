/*************************************************************************/
/*  audio_server.hpp                                                     */
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

#include "system/system.hpp"
#include "math/vector3.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#include <vector>

namespace sun {

class Context;
class SoundSource;

/*struct audio_listener2D
{
    vector2f    position;
    vector2f    velocity;
    vector2f    orientation;
    vector2f    up_vector;
};*/

struct AudioListener3D
{
    Vector3f    position;
    Vector3f    velocity;
    Vector3f    orientation;
    Vector3f    up_vector;
};

class SUN_API AudioServer final : public System
{
public:

    SUN_SYSTEM_TYPE(AudioServer);

    explicit AudioServer(Context&);

    ~AudioServer() = default;

    bool init() override;

    void shutdown() override;

    void set_global_volume(float volume);

    void set_listener(const AudioListener3D& listener);

    void set_listener_position(Vector3f pos);

    void set_listener_velocity(Vector3f vel);

    void set_listener_orientation(Vector3f ori, Vector3f up_vec);

    int get_format_from_channel_count(uint count) const;

    inline float get_global_volume() { return volume_; }

    inline const AudioListener3D& get_listener() const { return listener_; }

private:

    AudioListener3D    listener_;
    float               volume_;

    const char* get_al_error(ALenum error) const noexcept;
    const char* get_alc_error(ALCenum error) const noexcept;

    ALCdevice*  alc_device_;
    ALCdevice*  alc_capture_device_;
    ALCcontext* alc_context_;

    std::vector<SoundSource*>  sound_sources_;

    // system functions

    Component* create_component_(uint type_hash, uint id) override;

    bool handles_component_(uint type_hash) override;
};

}
