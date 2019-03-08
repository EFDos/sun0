/*************************************************************************/
/*  vorbis_decoder.cpp                                                   */
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
#include "vorbis_decoder.hpp"

#include "core/filesys/input_stream.hpp"
#include "core/logger.hpp"

#include <cassert>

namespace sun {

size_t vorbis_decoder::callback_read(void* ptr, size_t size, size_t nmemb, void* data)
{
    return 0;
}

int vorbis_decoder::callback_seek(void* data, ogg_int64_t offset, int whence)
{
    return 0;
}

long vorbis_decoder::callback_tell(void* data)
{
    return 0;
}

ov_callbacks vorbis_decoder::callbacks_ = {
    &vorbis_decoder::callback_read,
    &vorbis_decoder::callback_seek,
    nullptr,
    &vorbis_decoder::callback_tell
};

vorbis_decoder::vorbis_decoder() : channel_count_(0)
{
    vorbis_.datasource = nullptr;
}

vorbis_decoder::~vorbis_decoder()
{
    close_();
}

bool vorbis_decoder::open(filesys::input_stream& stream, info& i)
{
    int status = ov_open_callbacks(&stream, &vorbis_, nullptr, 0, callbacks_);

    if (status < 0) {
        sun_logf_error("Vorbis decoder failed to open file: %s",
            stream.get_filepath().c_str());
        return false;
    }

    vorbis_info* v_info = ov_info(&vorbis_, -1);
    i.channel_count = v_info->channels;
    i.sample_rate = v_info->rate;
    i.sample_count = static_cast<size_t>(ov_pcm_total(&vorbis_, -1) *
         v_info->channels);

    channel_count_ = i.channel_count;

    return true;
}

void vorbis_decoder::seek(uint64 sample_offset)
{
    if (vorbis_.datasource == nullptr) {
        sun_log_fatal("Vorbis can't decode null data source!");
    }
    assert(vorbis_.datasource);

    ov_pcm_seek(&vorbis_, sample_offset / channel_count_);
}

uint64 vorbis_decoder::read(int16* samples, uint64 max)
{
    if (vorbis_.datasource == nullptr) {
        sun_log_fatal("Vorbis can't decode null data source!");
    }
    assert(vorbis_.datasource);

    uint64 count = 0;

    while (count < max)
    {
        int bytes_to_read = static_cast<int>(max - count) * sizeof(int16);
        long bytes_read = ov_read(&vorbis_, reinterpret_cast<char*>(samples),
            bytes_to_read, 0, 2, 1, nullptr);

        if (bytes_read > 0) {
            long samples_read = bytes_read / sizeof(int16);
            count += samples_read;
            samples += samples_read;
        } else {
            break;
        }
    }

    return count;
}

void vorbis_decoder::close_()
{
    if (vorbis_.datasource != nullptr) {
        ov_clear(&vorbis_);
        vorbis_.datasource = nullptr;
        channel_count_ = 0;
    }
}

bool vorbis_decoder::check(filesys::input_stream& stream)
{
    OggVorbis_File file;
    if (ov_test_callbacks(&stream, &file, nullptr, 0, callbacks_) == 0) {
        ov_clear(&file);
        return true;
    } else {
        return false;
    }
}

}
