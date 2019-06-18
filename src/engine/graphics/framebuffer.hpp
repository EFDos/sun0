/*************************************************************************/
/*  framebuffer.hpp                                                      */
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

#include "gpu_object.hpp"

namespace sun {

class Texture;

class SUN_API Framebuffer : public GPUObject
{
public:

    enum class Target
    {
        Read,
        Draw,
        ReadAndDraw
    };

    enum class Status
    {
        Null,
        Incomplete,
        Ok
    };

    virtual ~Framebuffer();

    virtual void attach_texture(Texture*) = 0;

    virtual void detach_texture() = 0;

    inline Target get_target() const {
        return target_;
    }

    inline Status get_status() const {
        return status_;
    }

protected:

    Framebuffer(Target t = Target::ReadAndDraw);

    Target  target_;
    Status  status_;

};

} // sun
