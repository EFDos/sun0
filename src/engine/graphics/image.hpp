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
#include "resources/resource.hpp"

#include <string>

namespace sun {

class SUN_API Image : public Resource
{
public:

    SUN_RESOURCE_TYPE(Image)

    Image(Context&, const std::string& = "");

    Image(const Image&);

    Image(Image&&) = default;

    bool load(const std::string&) override;

    void clear() override;

    void allocate(uint width, uint height);

    void allocate(const Vector2u& size);

    void set_data(const Vector2u& size, const ubyte* data);

    inline const uint8* get_data() const { return data_; }

    inline const Vector2u& get_size() const { return size_; }

    Image& operator=(const Image&);

    Image& operator=(Image&&) = default;

private:

    ubyte*      data_;
    Vector2u    size_;
};

}
