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

class SUN_API shader_stage final : public sun::shader_stage
{
public:

    shader_stage(const std::string& source, type t);

    ~shader_stage();

    // implements sun::shader_stage

    status compile() override;

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

class SUN_API shader final : public sun::shader
{
public:

    shader(shader_stage* vertex, shader_stage* fragment);

    ~shader();

    // implements sun::shader

    status build() override;

    // implements sun::gpu_object

    void release() override;

    void bind() const override;

    void unbind() const override;

    std::string get_warnings() const override;

private:

    void linking_check_() override;

    uint    id_;
};

} // opengl
} // sun
