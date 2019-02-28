/*************************************************************************/
/*  opengl/shader.cpp                                                    */
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
#include "shader.hpp"
#include "common/opengl.hpp"
#include "core/logger.hpp"

namespace sun {
namespace opengl {

// shader_stage implementation

constexpr GLenum get_gl_type(shader_stage::type t)
{
    switch (t) {
        case shader_stage::type::vertex:    return GLenum(GL_VERTEX_SHADER);
        case shader_stage::type::fragment:  return GLenum(GL_FRAGMENT_SHADER);
        case shader_stage::type::geometry:  return GLenum(GL_GEOMETRY_SHADER);
        default: return GLenum();
    }
}

shader_stage::shader_stage(const std::string& source, type t)
:   sun::shader_stage(source, t), id_(0)
{
    if (source.empty()) {
        return;
    }

    id_ = glCreateShader(get_gl_type(type_));

    status_ = status::compile_ready;
}

shader_stage::~shader_stage()
{
    release();
}

shader_stage::status shader_stage::compile()
{
    if (id_ == 0 || status_ != status::compile_ready) {
        return status_;
    }

    const char* c_src = source_.c_str();
    glShaderSource(id_, 1, &c_src, nullptr);
    glCompileShader(id_);

    compile_check_();

    return status_;
}

void shader_stage::release()
{
    if (id_ != 0) {
        glDeleteShader(id_);
    }
}

void shader_stage::compile_check_()
{
    GLint result = -1;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &result);

    if (!result) {
        status_ = status::compile_fail;
    } else {
        status_ = status::compile_ok;
    }
}

std::string shader_stage::get_warnings() const
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

// shader implementation

shader::shader(shader_stage* vertex, shader_stage* fragment)
:   sun::shader(vertex, fragment),
    id_(0)
{
    id_ = glCreateProgram();
}

shader::~shader()
{
    release();
}

shader::status shader::build()
{
    if (id_ == 0) {
        sun_log_error("Internal OpenGL Shader failed to be created.");
    }

    if (vertex_stage_ == nullptr || fragment_stage_ == nullptr) {
        sun_log_error("Shader stages are not available for linking.");
        return status_ = status::invalid;
    }

    auto vert_stg = dynamic_cast<shader_stage*>(vertex_stage_.get());
    auto frag_stg = dynamic_cast<shader_stage*>(fragment_stage_.get());

    glAttachShader(id_, vert_stg->get_internal_id());
    glAttachShader(id_, frag_stg->get_internal_id());

    glLinkProgram(id_);

    linking_check_();

    glDetachShader(id_, vert_stg->get_internal_id());
    glDetachShader(id_, frag_stg->get_internal_id());

    return status_;
}

void shader::attach() const
{
    glUseProgram(id_);
}

void shader::release()
{
    if (id_ != 0) {
        glDeleteProgram(id_);
    }
}

void shader::linking_check_()
{
    GLint result = -1;
    glGetProgramiv(id_, GL_LINK_STATUS, &result);

    if (!result) {
        status_ = status::invalid;
    } else {
        status_ = status::ok;
    }
}

std::string shader::get_warnings() const
{
    GLint length = 0;

    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return std::string();
    }

    GLchar* info = nullptr;

    info = new GLchar[length];

    glGetProgramInfoLog(id_, length, &length, info);

    std::string ret("Error linking shader:\n"
        "\n\nGLError: " + std::string(static_cast<const char*>(info)));

    delete [] info;

    return ret;
}

} // opengl
} // sun
