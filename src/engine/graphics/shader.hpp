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

#include <string>

namespace sun {

class shader_stage
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
        compile_warning,
        compile_fail
    };

    shader_stage(const std::string& source, type t);

    virtual ~shader_stage();

    shader_stage(const shader_stage&) = delete;

    shader_stage& operator=(const shader_stage&) = delete;

    virtual void compile() = 0;

    virtual std::string get_warnings() const = 0;

    inline status get_status() const { return status_; }

protected:

    virtual void compile_check_() const = 0;

    std::string source_;
    type        type_;
    status      status_;
};

class shader
{
public:

    shader(const shader_stage& vertex, const shader_stage& fragment);

    virtual void attach() = 0;

    virtual void build() = 0;

    virtual std::string get_warnings() const = 0;

};

}
