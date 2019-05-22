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

#include "core/logger.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <cstring>

namespace sun {

image::image(context& p_context, const std::string& path)
:   resource(p_context),
    data_(nullptr)
{
    if (!path.empty()) {
        load(path);
    }
}

image::image(const image& other)
:   resource(other.context_)
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

bool image::load(const std::string& path)
{
    clear();

    int width, height, channels;

    ubyte* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

    #ifdef SUN_OUT_DEBUG
        sun_logf_debug("STB image loaded\nwidth: %d\nheight: %d\nbpp: %d",
            width, height, channels);
    #endif

    if (data == nullptr) {
        sun_logf_error("Error loading image: %s", path.c_str());
        return false;
    }
    //TODO: DISCOVER FUCKING WHY THIS FUNCTION CALL KILLS EVERYTHING
    //set_data({static_cast<uint>(width), static_cast<uint>(height)}, data);

    allocate(width, height);
    std::memcpy(data_, data, width * height * 4);

    stbi_image_free(data);

    return resource::load(path);
}

void image::allocate(uint width, uint height)
{
    clear();

    data_ = new ubyte[width * height * 4];
    size_ = {width, height};
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
    resource::clear();

    if (data_ != nullptr) {
        delete [] data_;
        data_ = nullptr;
        size_ = {0, 0};
    }
}

}
