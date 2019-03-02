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
#include "common/int.hpp"
#include "gpu_object.hpp"

namespace sun {

class image;

class SUN_API texture : public gpu_object
{
public:

    enum class filter_mode
    {
        nearest = 0,
        bilinear,
        trilinear,
        anisotropic,
        nearest_anisotropic
    };


    enum class address_mode
    {
        wrap = 0,
        mirror,
        clamp,
        border,
    };

    enum class usage
    {
        static_usage = 0,
        dynamic,
        render_target,
        depth_stencil
    };

    texture();

    virtual ~texture() = default;

    virtual void load(const image& img) = 0;

    virtual void load(const ubyte* data) = 0;

    inline void set_filter_mode(filter_mode mode) { filter_mode_ = mode; }

    inline void set_address_mode(address_mode mode) { address_mode_ = mode; }

    inline void set_usage(usage usg) { usage_ = usg; }

    inline filter_mode get_filter_mode() const { return filter_mode_; }

    inline address_mode get_address_mode() const { return address_mode_; }

    inline usage get_usage() const { return usage_; }

protected:

    filter_mode     filter_mode_;
    address_mode    address_mode_;
    usage           usage_;

};

}
