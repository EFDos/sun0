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
#include "common/types.hpp"
#include "core/logger.hpp"

namespace sun {
namespace opengl {

// ShaderStage implementation

constexpr GLenum get_gl_type(ShaderStage::Type t)
{
    switch (t) {
        case ShaderStage::Type::Vertex:    return GLenum(GL_VERTEX_SHADER);
        case ShaderStage::Type::Fragment:  return GLenum(GL_FRAGMENT_SHADER);
        case ShaderStage::Type::Geometry:  return GLenum(GL_GEOMETRY_SHADER);
        default: return GLenum();
    }
}

ShaderStage::ShaderStage(const std::string& source, Type t)
:   sun::ShaderStage(source, t), id_(0)
{
    if (source.empty()) {
        return;
    }

    id_ = glCreateShader(get_gl_type(type_));

    status_ = Status::CompileReady;
}

ShaderStage::~ShaderStage()
{
    release();
}

ShaderStage::Status ShaderStage::compile()
{
    if (id_ == 0 || status_ != Status::CompileReady) {
        return status_;
    }

    const char* c_src = source_.c_str();
    glShaderSource(id_, 1, &c_src, nullptr);
    glCompileShader(id_);

    compile_check_();

    return status_;
}

void ShaderStage::release()
{
    if (id_ != 0) {
        glDeleteShader(id_);
    }
}

void ShaderStage::compile_check_()
{
    GLint result = -1;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &result);

    if (!result) {
        status_ = Status::CompileFail;
    } else {
        status_ = Status::CompileOk;
    }
}

std::string ShaderStage::get_warnings() const
{
    GLint length = 0;

    glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

    if (length == 0) {
        return std::string();
    }

    GLchar* info = nullptr;
    std::string type_str;

    switch(type_) {
        case Type::Vertex:
            type_str = "vertex";
            break;
        case Type::Fragment:
            type_str = "fragment";
            break;
        case Type::Geometry:
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

Shader::Shader(ShaderStage* vertex, ShaderStage* fragment)
:   sun::Shader(vertex, fragment),
    id_(0)
{
    if (vertex_stage_ != nullptr && fragment_stage_ != nullptr) {
        id_ = glCreateProgram();
    }
}

Shader::~Shader()
{
    release();
}

Shader::Status Shader::build()
{
    if (id_ == 0) {
        sun_log_error("Internal OpenGL Shader failed to be created.");
    }

    if (vertex_stage_ == nullptr || fragment_stage_ == nullptr) {
        sun_log_error("Shader stages are not available for linking.");
        return status_ = Status::Invalid;
    }

    auto vert_stg = dynamic_cast<ShaderStage*>(vertex_stage_.get());
    auto frag_stg = dynamic_cast<ShaderStage*>(fragment_stage_.get());

    glAttachShader(id_, vert_stg->get_internal_id());
    glAttachShader(id_, frag_stg->get_internal_id());

    glLinkProgram(id_);

    linking_check_();

    glDetachShader(id_, vert_stg->get_internal_id());
    glDetachShader(id_, frag_stg->get_internal_id());

    return status_;
}

void Shader::bind() const
{
    if (id_ == 0) {
        sun_log_error("Failed to bind shader: Shader in invalid state.");
        return;
    }

    glUseProgram(id_);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::release()
{
    if (id_ != 0) {
        glDeleteProgram(id_);
    }
}

void Shader::linking_check_()
{
    if (id_ == 0) {
        sun_log_error("Failed to check linking on shader:"
                      " Shader in invalid state.");
        return;
    }

    GLint result = -1;
    glGetProgramiv(id_, GL_LINK_STATUS, &result);

    if (!result) {
        status_ = Status::Invalid;
    } else {
        status_ = Status::Ok;
    }
}

void Shader::send(const std::string& name, const Matrix4& mat4)
{
    if (id_ == 0) {
        sun_log_error("Failed to set shader uniform: Shader in invalid state.");
        return;
    }

    glUseProgram(id_);
    glUniformMatrix4fv(glGetUniformLocation(id_, name.c_str()), 1, GL_FALSE, mat4.get_data());
    glUseProgram(0);
}

void Shader::send(const std::string& name, int v)
{
    if (id_ == 0) {
        sun_log_error("Failed to set shader uniform: Shader in invalid state.");
        return;
    }

    glUseProgram(id_);
    glUniform1i(glGetUniformLocation(id_, name.c_str()), v);
    glUseProgram(0);
}

void Shader::send(const std::string& name, const Vector2f& vec2)
{
    if (id_ == 0) {
        sun_log_error("Failed to set shader uniform: Shader in invalid state.");
        return;
    }

    glUseProgram(id_);
    glUniform2f(glGetUniformLocation(id_, name.c_str()), vec2.x, vec2.y);
    glUseProgram(0);
}

void Shader::send(const std::string& name, const Colorf& color)
{
    if (id_ == 0) {
        sun_log_error("Failed to set shader uniform: Shader in invalid state.");
        return;
    }
    glUseProgram(id_);
    glUniform3f(glGetUniformLocation(id_, name.c_str()), color.r, color.g,
        color.b);
    glUseProgram(0);
}

void Shader::send(const std::string& name, float v)
{
    if (id_ == 0) {
        sun_log_error("Failed to set shader uniform: Shader in invalid state.");
        return;
    }

    glUseProgram(id_);
    glUniform1f(glGetUniformLocation(id_, name.c_str()), v);
    glUseProgram(0);
}

std::string Shader::get_warnings() const
{
    if (id_ == 0) {
        sun_log_error("Failed to get shader warnings:"
                      " Shader in invalid state.");
        return "";
    }

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
