/*************************************************************************/
/*  opengl/texture.hpp                                                   */
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

#include "graphics/texture.hpp"

namespace sun {
namespace opengl {

class SUN_API Texture final : public sun::Texture
{
public:

    Texture(Context&);

    ~Texture();

    // implements gpu_object

    void release() override;

    void bind() const override;

    void unbind() const override;

    // implements sun::texture

    bool load(const Image& img) override;

    bool load(const Vector2u& size, const ubyte* data) override;

    void resize(const Vector2u& size) override;

    void fill(const Vector2u& offset, const Vector2u& size,
              const ubyte* date) override;

    void clear() override;

    void map() override;

    void unmap() override;

    void set_filter_mode(FilterMode mode) override;

    void set_address_mode(AddressMode mode) override;

    uint get_internal_id() const {
        return id_;
    }

private:

    uint    id_;

};

} // opengl
} // sun
