/*************************************************************************/
/*  texture.cpp                                                          */
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
#include "image.hpp"
#include "core/logger.hpp"

namespace sun {

Texture::Texture(Context& context)
:   Resource(context),
    filter_mode_(FilterMode::Nearest),
    address_mode_(AddressMode::Wrap),
    usage_(Usage::Static),
    format_(Format::Rgba),
    map_buffer_(nullptr)
{
}

bool Texture::load(const std::string& path)
{
    Image img(context_);

    if (img.load(path)) {
        if (load(img)) {
            return Resource::load(path);
        }
    }
    sun_logf_error("Error loading texture from image: %s", path.c_str());
    return false;
}

}
