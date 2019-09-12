/*************************************************************************/
/*  texture.hpp                                                          */
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
#include "gpu_object.hpp"

namespace sun {

class Image;

class SUN_API Texture : public GPUObject, public Resource
{
public:

    SUN_RESOURCE_TYPE(Texture)

    enum class FilterMode
    {
        Nearest = 0,
        Bilinear,
        Trilinear,
        Anisotropic,
        NearestAnisotropic
    };


    enum class AddressMode
    {
        Wrap = 0,
        Mirror,
        Clamp,
        Border,
    };

    enum class Usage
    {
        Static = 0,
        Dynamic,
        RenderTarget,
        DepthStencil
    };

    enum class Format
    {
        Rgba,
        Bgra,
        Red,
        Green,
        Blue,
        Alpha
    };

    virtual ~Texture() = default;

    virtual bool load(const std::string&) override;

    virtual bool load(const Image& img) = 0;

    virtual bool load(const Vector2u& size, const ubyte* data) = 0;

    virtual void resize(const Vector2u& size) = 0;

    virtual void fill(const Vector2u& offset, const Vector2u& size,
                      const ubyte* data) = 0;

    //virtual void clear() = 0;

    virtual void map() = 0;

    virtual void unmap() = 0;

    inline void set_format(Format fmt) { format_ = fmt; }

    inline virtual void set_filter_mode(FilterMode mode) { filter_mode_ = mode; }

    inline virtual void set_address_mode(AddressMode mode) { address_mode_ = mode; }

    inline void set_usage(Usage usg) { usage_ = usg; }

    inline FilterMode get_filter_mode() const { return filter_mode_; }

    inline AddressMode get_address_mode() const { return address_mode_; }

    inline Usage get_usage() const { return usage_; }

    inline Format get_format() const { return format_; }

    inline const Vector2u& get_size() const { return size_; }

    inline const uint8* get_data() const { return map_buffer_; }

protected:

    Texture(Context&);

    FilterMode      filter_mode_;
    AddressMode     address_mode_;
    Usage           usage_;
    Format          format_;

    Vector2u        size_;

    uint8*          map_buffer_;
};

}
