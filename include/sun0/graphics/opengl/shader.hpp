/*************************************************************************/
/*  opengl/shader.hpp                                                    */
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

#include "graphics/shader.hpp"
#include "common/int.hpp"

namespace sun {
namespace opengl {

class SUN_API ShaderStage final : public sun::ShaderStage
{
public:

    ShaderStage(const std::string& source, Type t);

    ~ShaderStage();

    // implements sun::shader_stage

    Status compile() override;

    std::string get_warnings() const override;

    // implements sun::gpu_object

    void release() override;

    /* not used in this context*/
        void bind() const override {}

        void unbind() const override {}
    /* not used in this context */

    uint get_internal_id() const { return id_; }

private:

    void compile_check_() override;

    uint    id_;
};

class SUN_API Shader final : public sun::Shader
{
public:

    Shader(ShaderStage* vertex, ShaderStage* fragment);

    ~Shader();

    // implements sun::shader

    Status build() override;

    // implements sun::gpu_object

    void release() override;

    void bind() const override;

    void unbind() const override;

    void send(const std::string& name, const Matrix4& mat4) override;

    void send(const std::string& name, int v) override;

    void send(const std::string& name, const Vector2<float>& vec2) override;

    void send(const std::string& name, const BaseColor<float>& color) override;

    void send(const std::string& name, float v) override;

    std::string get_warnings() const override;

private:

    void linking_check_() override;

    uint    id_;
};

} // opengl
} // sun
