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

#include "common/time.hpp"
#include "sound_source.hpp"
#include "core/filesys/input_stream.hpp"

#include <vector>
#include <thread>
#include <mutex>

namespace sun {

class context;
class decoder;

class SUN_API sound_stream final : public sound_source
{
public:

    SUN_COMPONENT_TYPE(sound_stream)

    template<typename T>
    struct span
    {
        T offset;
        T length;

        span() {}
        span(T off, T len) : offset(off), length(len) {}
    };

    typedef span<time> time_span;

    struct chunk {
        const int16*    samples;
        size_t          sample_count;
    };

    sound_stream(context& p_context);

    ~sound_stream();

    bool load(const std::string&);

    void play() override;

    void pause() override;

    void stop() override;

    void set_playing_offset(time);

    void set_loop_points(time_span points);

    inline void set_loop(bool loop) {
        loop_ = loop;
    }

    time get_playing_offset() const;

    time get_duration() const;

    time_span get_loop_points() const;

    state get_state() const override;

    inline bool get_loop() const {
        return loop_;
    }

    inline uint get_channel_count() const {
        return channel_count_;
    }

    inline uint get_sample_rate() const {
        return sample_rate_;
    }

private:

    enum {
        no_loop = -1
    };

    void initialize_();

    bool on_get_data_(chunk& data);

    void on_seek_(time);

    int64 on_loop_();

    void stream_data_();

    bool fill_and_push_buffer_(uint buffer_n, bool immediate_loop = false);

    bool fill_queue_();

    void clear_queue_();

    uint64 time_to_samples_(time pos) const;

    time samples_to_time(uint64 samples) const;

    enum {
        buffer_count = 3,
        buffer_retries = 2
    };

    std::thread             thread_;
    mutable std::mutex      thread_mutex_;

    state           thread_start_state_;
    uint            channel_count_;
    uint            sample_rate_;
    uint32          format_;
    uint64          samples_processed_;
    bool            is_streaming_;
    bool            loop_;
    uint            buffers_[buffer_count];
    int64           buffer_seeks_[buffer_count];

    filesys::input_stream   file_;
    decoder*                decoder_;
    std::vector<int16>      samples_;
    std::mutex              file_mutex_;
    span<uint64>            loop_span_;
};

}
