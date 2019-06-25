/*************************************************************************/
/*  opengl/framebuffer.hpp                                               */
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

#include "graphics/framebuffer.hpp"
#include "common/int.hpp"

namespace sun {
namespace opengl {

class Framebuffer final : public sun::Framebuffer
{
public:

    Framebuffer(Target target);

    ~Framebuffer();

    void bind() const override;

    void unbind() const override;

    void release() override;

    void attach_texture(sun::Texture*) override;

    void detach_texture() override;

    inline uint get_internal_id() const {
        return id_;
    }

private:

    uint id_;
};

}
}