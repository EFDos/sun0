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

texture::texture(context& p_context)
:   resource(p_context),
    filter_mode_(filter_mode::nearest),
    address_mode_(address_mode::wrap),
    usage_(usage::static_usage),
    format_(format::rgba),
    map_buffer_(nullptr)
{
}

bool texture::load(const std::string& path)
{
    image img(context_);

    if (img.load(path)) {
        if (load(img)) {
            return resource::load(path);
        }
    }
    sun_logf_error("Error loading texture from image: %s", path.c_str());
    return false;
}

}
