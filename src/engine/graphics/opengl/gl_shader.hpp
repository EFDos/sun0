/*************************************************************************/
/*  gl_shader.hpp                                                        */
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

class SUN_API gl_shader_stage final : public shader_stage
{
public:

    gl_shader_stage(const std::string& source, type t);

    ~gl_shader_stage();

    status compile() override;

    std::string get_warnings() const override;

    uint get_internal_id() const { return id_; }

private:

    void compile_check_() override;

    uint    id_;
};

class SUN_API gl_shader final : public shader
{
public:

    gl_shader(gl_shader_stage* vertex, gl_shader_stage* fragment);

    ~gl_shader();

    status build() override;

    void attach() override;

    std::string get_warnings() const override;

private:

    void linking_check_() override;

    uint    id_;
};

}
