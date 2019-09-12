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

struct GLFilter {
    GLenum min;
    GLenum mag;
};

struct GLWrap {
    GLenum s;
    GLenum t;
};

constexpr GLenum get_gl_type(sun::Texture::Format fmt)
{
    switch (fmt) {
        case Texture::Format::Rgba:     return GL_RGBA;
        case Texture::Format::Bgra:     return GL_BGRA;
        case Texture::Format::Red:      return GL_RED;
        case Texture::Format::Blue:     return GL_BLUE;
        case Texture::Format::Green:    return GL_GREEN;
        case Texture::Format::Alpha:    return GL_ALPHA;
        default: return GLenum();
    }
}

constexpr GLFilter get_gl_filter(sun::Texture::FilterMode mode)
{
    switch (mode) {
        case Texture::FilterMode::Nearest:
            return {GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST};
        case Texture::FilterMode::Bilinear:
            return {GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR};
        case Texture::FilterMode::Trilinear:
            return {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR};
        default:
            return {GL_NEAREST, GL_NEAREST};
    }
}

constexpr GLWrap get_gl_wrap(sun::Texture::AddressMode mode)
{
    switch (mode) {
        case Texture::AddressMode::Clamp:
            return {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
        case Texture::AddressMode::Wrap:
            return {GL_REPEAT, GL_REPEAT};
        case Texture::AddressMode::Mirror:
            return {GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT};
        case Texture::AddressMode::Border:
            return {GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER};
        default:
            return {GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    }
}

constexpr int get_pixel_size(sun::Texture::Format fmt)
{
    switch (fmt) {
        case Texture::Format::Rgba:     return 4;
        case Texture::Format::Bgra:     return 4;
        case Texture::Format::Red:      return 1;
        case Texture::Format::Blue:     return 1;
        case Texture::Format::Green:    return 1;
        case Texture::Format::Alpha:    return 1;
        default: return 4;
    }
}

Texture::Texture(Context& context) : sun::Texture(context), id_(0)
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    set_filter_mode(FilterMode::Nearest);
    set_address_mode(AddressMode::Clamp);
}

Texture::~Texture()
{
    release();
}

void Texture::release()
{
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }
}

void Texture::bind() const
{
    if (id_ == 0) {
        sun_log_error("Error binding texture: Texture in invalid state.");
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Texture::load(const Image& img)
{
    return load(img.get_size(), img.get_data());
}

bool Texture::load(const Vector2u& size, const ubyte* data)
{
    if (id_ == 0) {
        sun_log_error("Error loading texture: Texture in invalid state.");
        return false;
    }

    if (data == nullptr) {
        sun_log_error("Error loading texture: data is null");
        return false;
    }

    size_ = size;
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, get_gl_type(format_), size.w, size.h, 0,
                 get_gl_type(format_), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::resize(const Vector2u& size)
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

void Texture::fill(const Vector2u& offset, const Vector2u& size,
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
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::clear()
{
    Resource::clear();

    if (id_ == 0) {
        sun_log_error("Error clearing texture: Texture in invalid state.");
        return;
    }

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, get_gl_type(format_), 0, 0, 0,
                 get_gl_type(format_), GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::map()
{
    unmap();

    map_buffer_ = new uint8[size_.x * size_.y * get_pixel_size(format_)];

    glBindTexture(GL_TEXTURE_2D, id_);
    glGetTexImage(GL_TEXTURE_2D, 0, get_gl_type(format_), GL_UNSIGNED_BYTE,
        map_buffer_);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::unmap()
{
    if(map_buffer_ == nullptr){
        return;
    }

    delete [] map_buffer_;
    map_buffer_ = nullptr;
}

void Texture::set_filter_mode(FilterMode mode)
{
    auto gl_filter = get_gl_filter(mode);

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter.min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter.mag);
    glBindTexture(GL_TEXTURE_2D, 0);

    sun::Texture::set_filter_mode(mode);
}

void Texture::set_address_mode(AddressMode mode)
{
    auto gl_wrap = get_gl_wrap(mode);

    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap.s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap.t);
    glBindTexture(GL_TEXTURE_2D, 0);

    sun::Texture::set_address_mode(mode);
}

} // opengl
} // sun
