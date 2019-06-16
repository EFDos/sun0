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

class Context;
class Decoder;

class SUN_API SoundStream final : public SoundSource
{
public:

    SUN_COMPONENT_TYPE(SoundStream)

    template<typename T>
    struct Span
    {
        T offset;
        T length;

        Span() {}
        Span(T off, T len) : offset(off), length(len) {}
    };

    typedef Span<Time> TimeSpan;

    struct Chunk {
        const int16*    samples;
        size_t          sample_count;
    };

    SoundStream(context& p_context);

    ~SoundStream();

    bool load(const std::string&);

    void play() override;

    void pause() override;

    void stop() override;

    void set_playing_offset(Time);

    void set_loop_points(TimeSpan points);

    inline void set_loop(bool loop) {
        loop_ = loop;
    }

    Time get_playing_offset() const;

    Time get_duration() const;

    TimeSpan get_loop_points() const;

    State get_state() const override;

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

    bool on_get_data_(Chunk& data);

    void on_seek_(time);

    int64 on_loop_();

    void stream_data_();

    bool fill_and_push_buffer_(uint buffer_n, bool immediate_loop = false);

    bool fill_queue_();

    void clear_queue_();

    uint64 time_to_samples_(Time pos) const;

    Time samples_to_time(uint64 samples) const;

    enum {
        buffer_count = 3,
        buffer_retries = 2
    };

    std::thread             thread_;
    mutable std::mutex      thread_mutex_;

    State           thread_start_state_;
    uint            channel_count_;
    uint            sample_rate_;
    uint32          format_;
    uint64          samples_processed_;
    bool            is_streaming_;
    bool            loop_;
    uint            buffers_[buffer_count];
    int64           buffer_seeks_[buffer_count];

    filesys::InputStream    file_;
    Decoder*                decoder_;
    std::vector<int16>      samples_;
    std::mutex              file_mutex_;
    Span<uint64>            loop_span_;
};

}
