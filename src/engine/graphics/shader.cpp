/*************************************************************************/
/*  shader.cpp                                                           */
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
#include "core/logger.hpp"

#include <sstream>

namespace sun {

shader_stage::shader_stage(const std::string& source, type t)
:   source_(source),
    type_(t),
    status_(status::invalid)
{
}

shader_stage::~shader_stage()
{
}

shader_utils::source_pair shader_utils::parse_source_pair(
    const std::string& source)
{
    std::string sources[2];

    enum { undef_src = -1, vertex_src = 0, fragment_src = 1};

    int type = undef_src;

    std::stringstream iss(source);

    for (std::string line ; std::getline(iss, line) ; )
    {
        if (line.find("#VERTEX") != std::string::npos) {
            type = static_cast<int>(shader_stage::type::vertex);
        } else if (line.find("#FRAGMENT") != std::string::npos) {
            type = static_cast<int>(shader_stage::type::fragment);
        }
        else
        {
            if (type == -1) {
                continue;
            }

            sources[type].append(line);
            sources[type].append("\n");
        }
    }

    return {sources[0], sources[1]};
}

shader::shader(shader_stage* vertex, shader_stage* fragment)
:   vertex_stage_(vertex),
    fragment_stage_(fragment),
    status_(status::invalid)
{
    if (vertex_stage_->get_status() == shader_stage::status::compile_ok &&
        fragment_stage_->get_status() == shader_stage::status::compile_ok)
    {
        return;
    }

    if (vertex_stage_->get_status() == shader_stage::status::compile_ready) {
        if (vertex_stage_->compile() != shader_stage::status::compile_ok) {
            sun_logf_error("%s", vertex_stage_->get_warnings().c_str());
            vertex_stage_ = nullptr;
        }
    } else {
        vertex_stage_ = nullptr;
    }

    if (fragment_stage_->get_status() == shader_stage::status::compile_ready) {
        if (fragment_stage_->compile() != shader_stage::status::compile_ok) {
            sun_logf_error("%s", fragment_stage_->get_warnings().c_str());
            fragment_stage_ = nullptr;
        }
    } else {
        fragment_stage_ = nullptr;
    }
}

shader::~shader()
{
}

}
