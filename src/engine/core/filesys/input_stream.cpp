/*************************************************************************/
/*  input_stream.cpp                                                      */
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
#include "input_stream.hpp"

#include "core/logger.hpp"

namespace sun {
namespace filesys {

InputStream::InputStream() noexcept : file_(nullptr)
{
    // do nothing
}

InputStream::~InputStream()
{
    if (file_ != nullptr) {
        std::fclose(file_);
    }
}

bool InputStream::open(const std::string& path)
{
    if (file_ != nullptr) {
        std::fclose(file_);
    }

    file_ = std::fopen(path.c_str(), "rb");

    if (file_ == nullptr) {
        sun_logf_error("Could not open input file %s for stream.",
            path.c_str());

        return false;
    }

    filepath_ = path;
    return true;
}

void InputStream::close()
{
    if (file_ != nullptr) {
        std::fclose(file_);
    }
}

int64 InputStream::read(void* buffer, int64 size) const
{
    if (file_ == nullptr) {
        sun_log_error("Can't read from closed stream.");
        return -1;
    }

    return std::fread(buffer, 1, static_cast<std::size_t>(size), file_);
}

int64 InputStream::seek(int64 position)
{
    if (file_ == nullptr) {
        sun_log_error("Can't seek on closed stream.");
        return -1;
    }

    std::fseek(file_, static_cast<size_t>(position), SEEK_SET);

    return tell();
}

int64 InputStream::tell() const
{
    if (file_ == nullptr) {
        sun_log_error("Can't tell from closed stream.");
        return -1;
    }

    return std::ftell(file_);
}

int64 InputStream::get_size() const
{
    if (file_ == nullptr) {
        sun_log_error("Can't get size from closed stream.");
        return -1;
    }

    size_t pos = std::ftell(file_);
    std::fseek(file_, 0, SEEK_END);
    int64 size = std::ftell(file_);
    std::fseek(file_, pos, SEEK_SET);

    return size;
}

} // filesys
} // sun
