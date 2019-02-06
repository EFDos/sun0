/*************************************************************************/
/*  gl_shader.cpp                                                        */
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
#include "gl_shader.hpp"
#include "common/opengl.hpp"

namespace sun {

gl_shader_stage::gl_shader_stage(const std::string& source, type t)
:   shader_stage(source, t), id_(0)
{
    if (source.empty()) {
        return;
    }

    GLenum gl_type;

    switch (t) {
        case type::vertex: gl_type = GL_VERTEX_SHADER; break;
        case type::fragment: gl_type = GL_FRAGMENT_SHADER; break;
        case type::geometry: gl_type = GL_GEOMETRY_SHADER; break;
    }

    id_ = glCreateShader(gl_type);

    status_ = status::compile_ready;
}

gl_shader_stage::~gl_shader_stage()
{
    if (id_ != 0) {
        glDeleteShader(id_);
        id_ = 0;
    }
}

void gl_shader_stage::compile()
{
    if (id_ == 0 || status_ != status::compile_ready) {
        return;
    }

    const char* c_src = source_.c_str();
    glShaderSource(id_, 1, &c_src, nullptr);
    glCompileShader(id_);

    compile_check_();
}

void gl_shader_stage::compile_check_() const
{

}

}
