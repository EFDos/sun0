/*************************************************************************/
/*  decoder.hpp                                                          */
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
#include "common/time.hpp"

namespace sun {

namespace filesys {

class input_stream;

}

class SUN_API decoder
{
public:

    struct info
    {
        uint64  sample_count;
        uint    channel_count;
        uint    sample_rate;
    };

    decoder() : sample_offset_(0) {}

    virtual ~decoder() {}

    virtual bool open(filesys::input_stream& stream) = 0;

    virtual void seek(uint64 sample_offset) = 0;

    virtual uint64 read(int16* samples, uint64 max) = 0;

    inline time get_duration() const
    {
        if (info_.channel_count == 0 || info_.sample_rate == 0) {
            return time::zero;
        }

        return time::seconds(static_cast<float>(info_.sample_count) /
            info_.channel_count / info_.sample_rate);
    }

    inline time get_time_offset() const
    {
        if (info_.channel_count == 0 || info_.sample_rate == 0) {
            return time::zero;
        }

        return time::seconds(static_cast<float>(sample_offset_) /
            info_.channel_count / info_.sample_rate);
    }

    inline uint64 get_sample_offset() const { return sample_offset_; }

    inline const info& get_info() const { return info_; }

protected:

    info    info_;
    uint64  sample_offset_;
};

} // sun
