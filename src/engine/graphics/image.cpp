/*************************************************************************/
/*  image.cpp                                                            */
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
#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstring>

namespace sun {

image::image() : data_(nullptr)
{
}

image::image(const image& other)
{
    if (other.data_ != nullptr) {
        set_data(other.size_, other.data_);
    }
}

image& image::operator=(const image& other)
{
    if (other.data_ != nullptr) {
        set_data(other.size_, other.data_);
    }

    return *this;
}

image::~image()
{
    clear();
}

void image::load(const std::string& path)
{
    int width, height, channels;

    ubyte* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    set_data({static_cast<uint>(width), static_cast<uint>(height)}, data);

    stbi_image_free(data);
}

void image::allocate(const vector2u& size)
{
    clear();

    data_ = new ubyte[size.w * size_.h * 4];
    size_ = size;
}

void image::set_data(const vector2u& size, const ubyte* data)
{
    allocate(size);
    std::memcpy(data_, data, size.w * size.h * 4);
}

void image::clear()
{
    if (data_ != nullptr) {
        delete [] data_;
        data_ = nullptr;
        size_ = {0, 0};
    }
}

}
