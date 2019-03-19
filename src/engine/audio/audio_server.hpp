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

//#include "math/vector2.hpp"
#include "math/vector3.hpp"

namespace sun {

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

class SUN_API audio_server : public system
{
public:

    SUN_SYSTEM_TYPE(SYS_AUDIO_SERVER);

    virtual ~audio_server() = default;

    virtual bool init() override;

    virtual void shutdown() override;

    inline virtual void set_global_volume(float volume) { volume_ = volume; }

    inline virtual void set_listener(const audio_listener3D& listener)
    {
        listener_ = listener;
    }

    inline virtual void set_listener_position(vector3f pos)
    {
        listener_.position = pos;
    }

    inline virtual void set_listener_velocity(vector3f vel)
    {
        listener_.velocity = vel;
    }

    inline virtual void set_listener_orientation(vector3f ori, vector3f up_vec)
    {
        listener_.orientation = ori;
        listener_.up_vector = up_vec;
    }

    //virtual int get_format_from_channel_count(uint channel_count) = 0;

    inline float get_global_volume() { return volume_; }

    inline const audio_listener3D& get_listener() const { return listener_; }

protected:

    audio_server() = default;

    audio_listener3D    listener_;
    float               volume_;
};

}
