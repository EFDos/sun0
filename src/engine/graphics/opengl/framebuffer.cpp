/*************************************************************************/
/*  opengl/framebuffer.cpp                                               */
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
#include "framebuffer.hpp"
#include "texture.hpp"
#include "common/opengl.hpp"
#include "core/logger.hpp"

namespace sun {
namespace opengl {

constexpr GLenum get_gl_type(Framebuffer::Target t)
{
    switch (t) {
        case Framebuffer::Target::Read:
            return GLenum(GL_READ_FRAMEBUFFER);
        case Framebuffer::Target::Draw:
            return GLenum(GL_DRAW_FRAMEBUFFER);
        case Framebuffer::Target::ReadAndDraw:
            return GLenum(GL_FRAMEBUFFER);
        default:
            return GLenum();
    }
}

Framebuffer::Framebuffer(Target target)
:   sun::Framebuffer(target)
{
    glGenFramebuffers(1, &id_);
    status_ = Status::Incomplete;
}

Framebuffer::~Framebuffer()
{
    release();
}

void Framebuffer::bind() const {
    if (id_ == 0 || status_ == Status::Null) {
        sun_log_error("Failed to bind framebuffer: invalid state.");
        return;
    }
    glBindFramebuffer(get_gl_type(target_), id_);
}

void Framebuffer::unbind() const {
    glBindFramebuffer(get_gl_type(target_), 0);
}

void Framebuffer::release() {
    if (id_ == 0) {
        return;
    }

    detach_texture();
    glDeleteFramebuffers(1, &id_);
    status_ = Status::Null;
}

void Framebuffer::attach_texture(sun::Texture* texture)
{
    auto gl_texture = static_cast<sun::opengl::Texture*>(texture);

    bind();

    glFramebufferTexture2D(get_gl_type(target_),
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        gl_texture->get_internal_id(), 0);

    if (glCheckFramebufferStatus(get_gl_type(target_)) != GL_FRAMEBUFFER_COMPLETE) {
        sun_log_error("framebuffer::attach_texture: Error creating Framebuffer."
                      " Incomplete fbo.");
        status_ = Status::Incomplete;
        unbind();
        return;
    }

    status_ = Status::Ok;
    unbind();
    return;
}

void Framebuffer::detach_texture()
{
    bind();

    glFramebufferTexture2D(get_gl_type(target_),
        GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        0, 0);

    status_ = Status::Incomplete;
    unbind();
}

}
}
