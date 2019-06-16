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

size_t VorbisDecoder::callback_read(void* ptr, size_t size, size_t nmemb, void* data)
{
    filesys::input_stream* stream = static_cast<filesys::input_stream*>(data);
    return static_cast<std::size_t>(stream->read(ptr, size * nmemb));
}

int VorbisDecoder::callback_seek(void* data, ogg_int64_t offset, int whence)
{
    filesys::input_stream* stream = static_cast<filesys::input_stream*>(data);
    switch (whence)
    {
        case SEEK_SET:
            break;
        case SEEK_CUR:
            offset += stream->tell();
            break;
        case SEEK_END:
            offset = stream->get_size() - offset;
    }
    return static_cast<int>(stream->seek(offset));
}

long VorbisDecoder::callback_tell(void* data)
{
    filesys::input_stream* stream = static_cast<filesys::input_stream*>(data);
    return static_cast<long>(stream->tell());
}

ov_callbacks VorbisDecoder::callbacks_ = {
    &VorbisDecoder::callback_read,
    &VorbisDecoder::callback_seek,
    nullptr,
    &VorbisDecoder::callback_tell
};

VorbisDecoder::VorbisDecoder()
{
    vorbis_.datasource = nullptr;
}

VorbisDecoder::~VorbisDecoder()
{
    close_();
}

bool VorbisDecoder::open(filesys::InputStream& stream)
{
    int status = ov_open_callbacks(&stream, &vorbis_, nullptr, 0, callbacks_);

    if (status < 0) {
        sun_logf_error("Vorbis decoder failed to open file: %s",
            stream.get_filepath().c_str());
        return false;
    }

    vorbis_info* v_info = ov_info(&vorbis_, -1);
    info_.channel_count = v_info->channels;
    info_.sample_rate = v_info->rate;
    info_.sample_count = static_cast<size_t>(ov_pcm_total(&vorbis_, -1) *
         v_info->channels);

    return true;
}

void VorbisDecoder::seek(uint64 sample_offset)
{
    if (vorbis_.datasource == nullptr) {
        sun_log_fatal("Vorbis can't decode null data source!");
    }
    assert(vorbis_.datasource);

    sample_offset_ = std::min(sample_offset, info_.sample_count);
    ov_pcm_seek(&vorbis_, sample_offset_ / info_.channel_count);
}

uint64 VorbisDecoder::read(int16* samples, uint64 max)
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
            sample_offset_ += samples_read;
        } else {
            break;
        }
    }

    return count;
}

void VorbisDecoder::close_()
{
    if (vorbis_.datasource != nullptr) {
        ov_clear(&vorbis_);
        vorbis_.datasource = nullptr;
        info_.channel_count = 0;
    }
}

bool VorbisDecoder::check(filesys::InputStream& stream)
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
