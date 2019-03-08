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

#include <string>
#include <memory>

namespace sun {

class matrix4;

class SUN_API shader_stage : public gpu_object
{
public:

    enum class type
    {
        vertex,
        fragment,
        geometry
    };

    enum class status
    {
        invalid,
        compile_ready,
        compile_ok,
        compile_fail
    };

    virtual ~shader_stage();

    virtual status compile() = 0;

    virtual std::string get_warnings() const = 0;

    inline status get_status() const { return status_; }

protected:

    shader_stage(const std::string& source, type t);

    virtual void compile_check_() = 0;

    std::string source_;
    type        type_;
    status      status_;
};

namespace shader_utils {

using source_pair = std::pair<std::string, std::string>;

source_pair parse_source_pair(const std::string& source);

}

class SUN_API shader : public gpu_object
{
public:

    enum class status {
        ok,
        invalid
    };

    virtual ~shader();

    virtual status build() = 0;

    virtual std::string get_warnings() const = 0;

    virtual void set_uniform(const std::string& name, const matrix4& mat4) = 0;

    virtual void set_uniform(const std::string& name, int v) = 0;

    inline status get_status() const { return status_; }

protected:

    shader(shader_stage* vertex, shader_stage* fragment);

    virtual void linking_check_() = 0;

    std::unique_ptr<shader_stage>   vertex_stage_;
    std::unique_ptr<shader_stage>   fragment_stage_;

    status status_;
};

}
