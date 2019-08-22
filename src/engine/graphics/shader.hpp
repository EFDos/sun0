/*************************************************************************/
/*  shader.hpp                                                           */
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

class Matrix4;
template<typename T>
class BaseColor;
template<typename T>
class Vector2;

class SUN_API ShaderStage : public GPUObject
{
public:

    enum class Type
    {
        Vertex,
        Fragment,
        Geometry
    };

    enum class Status
    {
        Invalid,
        CompileReady,
        CompileOk,
        CompileFail
    };

    virtual ~ShaderStage();

    virtual Status compile() = 0;

    virtual std::string get_warnings() const = 0;

    inline Status get_status() const { return status_; }

protected:

    ShaderStage(const std::string& source, Type t);

    virtual void compile_check_() = 0;

    std::string source_;
    Type        type_;
    Status      status_;
};

namespace ShaderUtils {

using SourcePair = std::pair<std::string, std::string>;

SourcePair parse_source_pair(const std::string& source);

}

class SUN_API Shader : public GPUObject
{
public:

    enum class Status {
        Ok,
        Invalid
    };

    virtual ~Shader();

    virtual Status build() = 0;

    virtual std::string get_warnings() const = 0;

    virtual void send(const std::string& name, const Matrix4& mat4) = 0;

    virtual void send(const std::string& name, const Vector2<float>& vec2) = 0;

    virtual void send(const std::string& name, const BaseColor<float>& color) = 0;

    virtual void send(const std::string& name, int v) = 0;

    virtual void send(const std::string& name, float v) = 0;

    inline Status get_status() const { return status_; }

protected:

    Shader(ShaderStage* vertex, ShaderStage* fragment);

    virtual void linking_check_() = 0;

    std::unique_ptr<ShaderStage>   vertex_stage_;
    std::unique_ptr<ShaderStage>   fragment_stage_;

    Status status_;
};

}
