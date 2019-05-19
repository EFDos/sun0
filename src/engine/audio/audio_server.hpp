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

class context;
class sound_source;

/*struct audio_listener2D
{
    vector2f    position;
    vector2f    velocity;
    vector2f    orientation;
    vector2f    up_vector;
};*/

struct audio_listener3D
{
    vector3f    position;
    vector3f    velocity;
    vector3f    orientation;
    vector3f    up_vector;
};

class SUN_API audio_server final : public system
{
public:

    SUN_SYSTEM_TYPE(SYS_AUDIO_SERVER);

    explicit audio_server(context&);

    ~audio_server() = default;

    bool init() override;

    void shutdown() override;

    void set_global_volume(float volume);

    void set_listener(const audio_listener3D& listener);

    void set_listener_position(vector3f pos);

    void set_listener_velocity(vector3f vel);

    void set_listener_orientation(vector3f ori, vector3f up_vec);

    int get_format_from_channel_count(uint count) const;

    inline float get_global_volume() { return volume_; }

    inline const audio_listener3D& get_listener() const { return listener_; }

protected:

    audio_listener3D    listener_;
    float               volume_;

    const char* get_al_error(ALenum error) const noexcept;
    const char* get_alc_error(ALCenum error) const noexcept;

    ALCdevice*  alc_device_;
    ALCdevice*  alc_capture_device_;
    ALCcontext* alc_context_;

    std::vector<sound_source*>  sound_sources_;

private:

    component* create_component_(uint type_hash) override;

    bool handles_component_(uint type_hash) override;
};

}
