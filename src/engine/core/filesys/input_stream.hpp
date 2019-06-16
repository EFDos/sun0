/*************************************************************************/
/*  input_stream.hpp                                                     */
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
#include "common/int.hpp"

#include <fstream>

namespace sun {
namespace filesys {

class SUN_API InputStream
{
public:

    InputStream() noexcept;

    ~InputStream();

    InputStream(const InputStream&) = delete;

    InputStream& operator=(const InputStream&) = delete;

    bool open(const std::string& path);

    void close();

    int64 read(void* buffer, int64 size) const;

    int64 seek(int64 position);

    int64 tell() const;

    int64 get_size() const;

    inline const std::string& get_filepath() const { return filepath_; }

private:

    std::FILE*   file_;
    std::string  filepath_;
};

} // filesys
} // sun
