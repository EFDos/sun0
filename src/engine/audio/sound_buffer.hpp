/*************************************************************************/
/*  sound_buffer.hpp                                                     */
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

class SUN_API SoundBuffer final : public SoundSource
{
public:

    SUN_COMPONENT_TYPE(SoundBuffer);

    SoundBuffer(Context&);

    ~SoundBuffer();

    void init() override;

    uint get_sample_rate() const;

    uint get_channel_count() const;

    inline uint64 get_sample_count() const {
        return sample_count_;
    }

    inline float get_duration() const {
        return duration_;
    }

    inline const std::vector<int16>& get_samples() const {
        return samples_;
    }

private:

    bool update(int channels, int sample_rate);

    uint                al_buffer_;
    std::vector<int16>  samples_;
    size_t              sample_count_;
    float               duration_;
};

}
