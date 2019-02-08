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
#include "core/logger.hpp"

namespace sun {

gl_shader_stage::gl_shader_stage(const std::string& source, type t)
:   shader_stage(source, t), id_(0)
{
    if (source.empty()) {
        return;
    }

    GLenum gl_type;

    switch (t) {
        case type::vertex:      gl_type = GL_VERTEX_SHADER; break;
        case type::fragment:    gl_type = GL_FRAGMENT_SHADER; break;
        case type::geometry:    gl_type = GL_GEOMETRY_SHADER; break;
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

void gl_shader_stage::compile_check_()
{
    GLint result = -1;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &result);

    if (!result) {
        status_ = status::compile_fail;
    } else {
        status_ = status::compile_ok;
    }
}

std::string gl_shader_stage::get_warnings() const
{
    GLint length = 0;

    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return std::string();
    }

    GLchar* info = nullptr;
    std::string type_str;

    switch(type_) {
        case type::vertex:
            type_str = "vertex";
            break;
        case type::fragment:
            type_str = "fragment";
            break;
        case type::geometry:
            type_str = "geometry";
    }

    info = new GLchar[length];

    glGetShaderInfoLog(id_, length, &length, info);

    std::string ret("Error compiling " + type_str + " shader:\n" + source_ +
        "\n\nGLError: " + std::string(static_cast<const char*>(info)));

    delete [] info;

    return ret;
}

}
