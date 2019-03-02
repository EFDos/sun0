/*************************************************************************/
/*  image.hpp                                                            */
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
#include "common/types.hpp"

#include <string>

namespace sun {

class SUN_API image
{
public:

    image();

    image(const image&);

    image(image&&) = default;

    ~image();

    void load(const std::string&);

    void allocate(uint width, uint height);

    void allocate(const vector2u& size);

    void set_data(const vector2u& size, const ubyte* data);

    void clear();

    inline const uint8* get_data() const { return data_; }

    inline const vector2u& get_size() const { return size_; }

    image& operator=(const image&);

    image& operator=(image&&) = default;

private:

    uint8*      data_;
    vector2u    size_;
};

}
