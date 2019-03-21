/*************************************************************************/
/*  openal/audio_server.hpp                                              */
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

#include "audio/audio_server.hpp"

#include <AL/al.h>
#include <AL/alc.h>

namespace sun {
namespace openal {

class SUN_API audio_server final : public sun::audio_server
{
public:

    audio_server(context&);

    ~audio_server() = default;

    bool init() override;

    void shutdown() override;

    void set_global_volume(float volume) override;

    void set_listener(const audio_listener3D& listener) override;

    void set_listener_position(vector3f pos) override;

    void set_listener_velocity(vector3f vel) override;

    void set_listener_orientation(vector3f ori, vector3f up_vec) override;

private:

    const char* get_al_error(ALenum error) const noexcept;
    const char* get_alc_error(ALCenum error) const noexcept;

    int format_from_channel_count(uint count) const;

    ALCdevice*  device_;
    ALCdevice*  capture_device_;
    ALCcontext* context_;

};

} // openal
} // sun
