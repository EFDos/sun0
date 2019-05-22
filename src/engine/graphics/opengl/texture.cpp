/*************************************************************************/
/*  opengl/texture.cpp                                                   */
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
#include "texture.hpp"

#include "core/logger.hpp"

#include "common/types.hpp"
#include "common/opengl.hpp"

#include "graphics/image.hpp"

namespace sun {
namespace opengl {

constexpr GLenum get_gl_type(sun::texture::format fmt)
{
    switch (fmt) {
        case texture::format::rgba:     return GLenum(GL_RGBA);
        case texture::format::bgra:     return GLenum(GL_BGRA);
        case texture::format::red:      return GLenum(GL_RED);
        case texture::format::blue:     return GLenum(GL_BLUE);
        case texture::format::green:    return GLenum(GL_GREEN);
        case texture::format::alpha:    return GLenum(GL_ALPHA);
        default: return GLenum();
    }
}

constexpr int get_pixel_size(sun::texture::format fmt)
{
    switch (fmt) {
        case texture::format::rgba:     return 4;
        case texture::format::bgra:     return 4;
        case texture::format::red:      return 1;
        case texture::format::blue:     return 1;
        case texture::format::green:    return 1;
        case texture::format::alpha:    return 1;
        default: return 4;
    }
}

texture::texture(context& p_context) : sun::texture(p_context), id_(0)
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, 0);
}

texture::~texture()
{
    release();
}

void texture::release()
{
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }
}

void texture::bind() const
{
    if (id_ == 0) {
        sun_log_error("Error binding texture: Texture in invalid state.");
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::load(const image& img)
{
    load(img.get_size(), img.get_data());
}

void texture::load(const vector2u& size, const ubyte* data)
{
    if (id_ == 0) {
        sun_log_error("Error loading texture: Texture in invalid state.");
        return;
    }

    if (data == nullptr) {
        sun_log_error("Error loading texture: data is null");
        return;
    }

    size_ = size;
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, get_gl_type(format_), size.w, size.h, 0,
                 get_gl_type(format_), GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::resize(const vector2u& size)
{
    if (id_ == 0) {
        sun_log_error("Failed to resize texture:"
                      " Texture in invalid state.");
        return;
    }

    size_ = size;
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, get_gl_type(format_), size.w, size.h, 0,
                 get_gl_type(format_), GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::fill(const vector2u& offset, const vector2u& size,
                   const ubyte* data)
{
    if (id_ == 0) {
        sun_log_error("Error filling texture: Texture in invalid state.");
        return;
    }

    if (data == nullptr) {
        sun_log_warn("Filling texture: data is null");
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size.w, size.h,
                    get_gl_type(format_), GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::clear()
{
    resource::clear();

    if (id_ == 0) {
        sun_log_error("Error clearing texture: Texture in invalid state.");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, get_gl_type(format_), 0, 0, 0,
                 get_gl_type(format_), GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::map()
{
    unmap();

    map_buffer_ = new uint8[size_.x * size_.y * get_pixel_size(format_)];

    glBindTexture(GL_TEXTURE_2D, id_);
    glGetTexImage(GL_TEXTURE_2D, 0, get_gl_type(format_), GL_UNSIGNED_BYTE,
        map_buffer_);
}

void texture::unmap()
{
    if(map_buffer_ == nullptr){
        return;
    }

    delete [] map_buffer_;
    map_buffer_ = nullptr;
}

} // opengl
} // sun
