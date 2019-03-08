/*************************************************************************/
/*  sound_stream.hpp                                                     */
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

#include "common/int.hpp"
#include "sound_source.hpp"

namespace sun {

class sound_stream : public sound_source
{
public:

    struct chunk {
        const int16*    samples;
        std::size_t     sample_count;
    };

    virtual ~sound_stream() {}

    void play() override;

    void pause() override;

    void stop() override;

    void load(filesys::input_stream& file);

    inline void set_loop(bool loop) { loop_ = loop; }

    //TODO: Add set/get for playing offset based on time
    //void set_playing_offset(some_time_struct);
    //some_time_struct get_playing_offset() const;

    uint get_channel_count() const { return channel_count_; }

    uint get_sample_rate() const { return sample_rate_; }

    inline bool get_loop() const { return loop_; }

protected:

    sound_stream();

    uint    channel_count_;
    uint    sample_rate_;
    bool    loop_;

    filesys::input_stream&  stream_;

};

}
