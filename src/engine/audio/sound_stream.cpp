/*************************************************************************/
/*  sound_stream.cpp                                                     */
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
#include "sound_stream.hpp"

#include "audio_server.hpp"
#include "decoders/vorbis_decoder.hpp"

#include "core/context.hpp"
#include "core/logger.hpp"

namespace sun {

SoundStream::SoundStream(Context& context)
:   SoundSource(context),
    thread_(),
    thread_start_state_(State::Stopped),
    channel_count_(0),
    sample_rate_(0),
    format_(0),
    samples_processed_(0),
    is_streaming_(false),
    loop_(false),
    decoder_(nullptr),
    loop_span_(0, 0)
{

}

SoundStream::~SoundStream()
{
    //stop();

    {
        std::lock_guard<std::mutex> lock(thread_mutex_);
        is_streaming_ = false;
    }

    if (thread_.joinable()) {
        thread_.join();
    }

    if (decoder_ != nullptr) {
        delete decoder_;
    }
}

bool SoundStream::load(const std::string& filepath)
{
    if (decoder_ != nullptr && get_state() == State::Playing) {
        stop();
    }

    //TODO: make decoder based on file type
    decoder_ = new VorbisDecoder();

    if (!file_.open(filepath) && decoder_ != nullptr) {
        return false;
    }

    return true;
}

void SoundStream::init()
{
    SoundSource::init();

    decoder_->open(file_);

    auto& info = decoder_->get_info();

    loop_span_.offset = 0;
    loop_span_.length = info.sample_count;

    samples_.resize(info.sample_rate * info.channel_count);

    channel_count_ = info.channel_count;
    sample_rate_ = info.sample_rate;
    samples_processed_ = 0;
    is_streaming_ = false;

    format_ = context_.get_system<AudioServer>()->
        get_format_from_channel_count(channel_count_);

    if (format_ == 0) {
        channel_count_ = 0;
        sample_rate_ = 0;
        sun_logf_error("Unsupported number of channels: %d", channel_count_);
    }
}

void SoundStream::play()
{
    if (format_ == 0 || decoder_ == nullptr) {
        sun_log_error("Failed to play sound stream: not initialized.");
        return;
    }

    bool is_streaming = false;
    State thread_start_state = State::Stopped;

    {
        std::lock_guard<std::mutex> lock(thread_mutex_);

        is_streaming = is_streaming_;
        thread_start_state = thread_start_state_;
    }

    if (is_streaming && thread_start_state == State::Paused) {
        std::lock_guard<std::mutex> lock(thread_mutex_);
        thread_start_state_ = State::Playing;
        alSourcePlay(source_);
        return;
    } else if (is_streaming && thread_start_state == State::Playing) {
        stop();
    }

    is_streaming_ = true;
    thread_start_state_ = State::Playing;
    thread_ = std::thread(&SoundStream::stream_data_, this);
}

void SoundStream::pause()
{
    {
        std::lock_guard<std::mutex> lock(thread_mutex_);

        if (!is_streaming_)
            return;

        thread_start_state_ = State::Paused;
    }

    alSourcePause(source_);
}

void SoundStream::stop()
{
    {
        std::lock_guard<std::mutex> lock(thread_mutex_);
        is_streaming_ = false;
    }

    if (thread_.joinable()) {
        thread_.join();
    }

    on_seek_(Time::ZERO);
}

SoundSource::State SoundStream::get_state() const
{
    auto st = SoundSource::get_state();

    if (st == State::Stopped) {
        std::lock_guard<std::mutex> lock(thread_mutex_);

        if (is_streaming_) {
            st = thread_start_state_;
        }
    }

    return st;
}

void SoundStream::set_playing_offset(Time offset)
{
    State old_state = get_state();

    stop();

    on_seek_(offset);

    samples_processed_ = static_cast<uint64>(offset.as_seconds() * sample_rate_ * channel_count_);

    if (old_state == State::Stopped) {
        return;
    }

    is_streaming_ = true;
    thread_start_state_ = old_state;
    thread_ = std::thread(&SoundStream::stream_data_, this);
}

void SoundStream::set_loop_points(TimeSpan points)
{

    if (decoder_ == nullptr) {
        sun_log_error("Stream is not in a valid state to set loop points");
        return;
    }

    Span<uint64> sample_points(time_to_samples_(points.offset),
        time_to_samples_(points.length));

    if (get_channel_count() == 0 || decoder_->get_info().sample_count == 0)
    {
        sun_log_error("Stream is not in a valid state to set loop points");
        return;
    }

    sample_points.offset += (get_channel_count() - 1);
    sample_points.offset -= (sample_points.offset % get_channel_count());
    sample_points.length += (get_channel_count() - 1);
    sample_points.length -= (sample_points.length % get_channel_count());

    if (sample_points.offset >= decoder_->get_info().sample_count)
    {
        sun_log_error("loop points offset must be in range [0, duration]");
        return;
    }

    if (sample_points.length == 0)
    {
        sun_log_error("loop points length val must be non zero");
        return;
    }

    sample_points.length = std::min(sample_points.length,
        decoder_->get_info().sample_count - sample_points.offset);

    if (sample_points.offset == loop_span_.offset &&
        sample_points.length == loop_span_.length)
    {
        return;
    }

    State old_state = get_state();
    Time old_pos = get_playing_offset();

    stop();

    loop_span_ = sample_points;

    if (old_pos != Time::ZERO) {
        set_playing_offset(old_pos);
    }

    if (old_state == State::Playing) {
        play();
    }
}

Time SoundStream::get_playing_offset() const
{
    if (sample_rate_ && channel_count_)
    {
        ALfloat secs = 0.f;
        alGetSourcef(source_, AL_SEC_OFFSET, &secs);

        return Time::seconds(secs + static_cast<float>(samples_processed_) / sample_rate_ / channel_count_);
    } else {
        return Time::ZERO;
    }
}

Time SoundStream::get_duration() const
{
    if (decoder_ == nullptr) {
        return Time::ZERO;
    }

    return decoder_->get_duration();
}

SoundStream::TimeSpan SoundStream::get_loop_points() const
{
    return TimeSpan(samples_to_time(loop_span_.offset),
        samples_to_time(loop_span_.length));
}

int64 SoundStream::on_loop_()
{
    std::lock_guard<std::mutex> lock(file_mutex_);

    uint64 current_offset = decoder_->get_sample_offset();
    if (get_loop() && loop_span_.length != 0 &&
        current_offset == loop_span_.offset + loop_span_.length)
    {
        decoder_->seek(loop_span_.offset);
        return decoder_->get_sample_offset();
    }
    else if (get_loop() && current_offset >= decoder_->get_info().sample_count)
    {
        decoder_->seek(0);
        return 0;
    }

    return no_loop;
}

bool SoundStream::on_get_data_(SoundStream::Chunk& data)
{
    std::lock_guard<std::mutex> lock(file_mutex_);

    size_t to_fill = samples_.size();
    uint64 current_offset = decoder_->get_sample_offset();
    uint64 loop_end = loop_span_.offset + loop_span_.length;

    // If the loop end is enabled and imminent, request less data.
    // This will trip an "onLoop()" call from the underlying SoundStream,
    // and we can then take action.
    if (get_loop() && loop_span_.length != 0 && current_offset <= loop_end &&
        current_offset + to_fill > loop_end)
    {
        to_fill = static_cast<size_t>(loop_end - current_offset);
    }

    data.samples = &samples_[0];
    data.sample_count = static_cast<size_t>(decoder_->read(&samples_[0],
        to_fill));
    current_offset += data.sample_count;

    return data.sample_count != 0 &&
        current_offset < decoder_->get_info().sample_count &&
        !(current_offset == loop_end && loop_span_.length != 0);
}

void SoundStream::on_seek_(Time offset)
{
    std::lock_guard<std::mutex> lock(file_mutex_);
    decoder_->seek(time_to_samples_(offset));
}

void SoundStream::stream_data_()
{
    bool request_stop = false;

    {
        std::lock_guard<std::mutex> lock(thread_mutex_);

        if (thread_start_state_ == State::Stopped) {
            is_streaming_ = false;
            return;
        }
    }

    alGenBuffers(buffer_count, buffers_);
    for (int i = 0 ; i < buffer_count ; ++i) {
        buffer_seeks_[i] = no_loop;
    }

    request_stop = fill_queue_();

    alSourcePlay(source_);

    {
        std::lock_guard<std::mutex> lock(thread_mutex_);

        if (thread_start_state_ == State::Paused) {
            alSourcePause(source_);
        }
    }

    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(thread_mutex_);

            if (!is_streaming_) {
                break;
            }
        }

        if (SoundSource::get_state() == State::Stopped)
        {
            if (!request_stop) {
                alSourcePlay(source_);
            } else {
                std::lock_guard<std::mutex> lock(thread_mutex_);
                is_streaming_ = false;
            }
        }
        ALint n_processed = 0;
        alGetSourcei(source_, AL_BUFFERS_PROCESSED, &n_processed);

        while (n_processed--)
        {
            ALuint buffer;
            alSourceUnqueueBuffers(source_, 1, &buffer);

            uint buffer_n = 0;
            for (int i = 0 ; i < buffer_count ; ++i)
            {
                if (buffers_[i] == buffer)
                {
                    buffer_n = i;
                    break;
                }
            }

            if (buffer_seeks_[buffer_n] != no_loop)
            {
                samples_processed_ = buffer_seeks_[buffer_n];
                buffer_seeks_[buffer_n] = no_loop;
            }
            else
            {
                ALint size, bits;
                alGetBufferi(buffer, AL_SIZE, &size);
                alGetBufferi(buffer, AL_BITS, &bits);

                if (bits == 0)
                {
                    sun_log_error("Bits in sound stream are 0:"
                        "audio format may be corrupted.");

                    std::lock_guard<std::mutex> lock(thread_mutex_);
                    is_streaming_ = false;
                    request_stop = true;
                    break;
                }
                else
                {
                    samples_processed_ += size / (bits / 8);
                }
            }

            if (!request_stop)
            {
                if (fill_and_push_buffer_(buffer_n))
                    request_stop = true;
            }
        }

        if (SoundSource::get_state() != State::Stopped) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    alSourceStop(source_);
    clear_queue_();

    samples_processed_ = 0;
    alSourcei(source_, AL_BUFFER, 0);
    alDeleteBuffers(buffer_count, buffers_);
}

bool SoundStream::fill_and_push_buffer_(uint buffer_n, bool immediate_loop)
{
    bool request_stop = false;

    Chunk data = {nullptr, 0};
    for (uint32 retry_count = 0 ; !on_get_data_(data) &&
        (retry_count < buffer_retries) ; ++retry_count)
    {
        if (!loop_)
        {
            if (data.samples != nullptr && data.sample_count != 0) {
                buffer_seeks_[buffer_n] = 0;
            }
            request_stop = true;
            break;
        }
        buffer_seeks_[buffer_n] = on_loop_();

        if (data.samples != nullptr && data.sample_count != 0) {
            break;
        }

        if (immediate_loop && (buffer_seeks_[buffer_n] != no_loop))
        {
            samples_processed_ = buffer_seeks_[buffer_n];
            buffer_seeks_[buffer_n] = no_loop;
        }
    }

    if (data.samples && data.sample_count)
    {
        uint buffer = buffers_[buffer_n];

        ALsizei size = static_cast<ALsizei>(data.sample_count) * sizeof(int16);
        alBufferData(buffer, format_, data.samples, size, sample_rate_);
        alSourceQueueBuffers(source_, 1, &buffer);
    } else {
        request_stop = true;
    }

    return request_stop;
}

bool SoundStream::fill_queue_()
{
    bool request_stop = false;
    for (int i = 0 ; i < buffer_count && !request_stop ; ++i) {
        if (fill_and_push_buffer_(i, i == 0)) {
            request_stop = true;
        }
    }

    return request_stop;
}

void SoundStream::clear_queue_()
{
    ALint n_queued;

    alGetSourcei(source_, AL_BUFFERS_QUEUED, &n_queued);

    ALuint buffer;
    for (ALint i = 0 ; i < n_queued ; ++i) {
        alSourceUnqueueBuffers(source_, 1, &buffer);
    }
}

uint64 SoundStream::time_to_samples_(Time pos) const
{
    // Taken from SFML
    // Always ROUND, no unchecked truncation, hence the addition in the numerator.
    // This avoids most precision errors arising from "samples => Time => samples" conversions
    // Original rounding calculation is ((Micros * Freq * Channels) / 1000000) + 0.5
    // We refactor it to keep Int64 as the data type throughout the whole operation.
    return ((pos.as_microseconds() * get_sample_rate() * get_channel_count()) + 500000) / 1000000;
}


Time SoundStream::samples_to_time(uint64 samples) const
{
    Time pos = Time::ZERO;

    // Make sure we don't divide by 0
    if (get_sample_rate() != 0 && get_channel_count() != 0)
        pos = Time::microseconds((samples * 1000000) /
            (get_channel_count() * get_sample_rate()));

    return pos;
}

}
