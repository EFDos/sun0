/*************************************************************************/
/*  vorbis_decoder.hpp                                                   */
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

#include "decoder.hpp"
#include <vorbis/vorbisfile.h>

namespace sun {

class SUN_API vorbis_decoder final : public decoder
{
public:

    vorbis_decoder();

    ~vorbis_decoder();

    // Implements decoder
    bool open(filesys::input_stream& stream, info& i) override;

    void seek(uint64 sample_offset) override;

    uint64 read(int16* samples, uint64 max) override;

    static bool check(filesys::input_stream& stream);

private:

    void close_();

    OggVorbis_File  vorbis_;
    uint            channel_count_;

    // Vorbis callbacks
    static size_t callback_read(void* ptr, size_t size, size_t nmemb, void* data);

    static int callback_seek(void* data, ogg_int64_t offset, int whence);

    static long callback_tell(void* data);

    static ov_callbacks callbacks_;

};

}
