/*************************************************************************/
/*  gl_renderer.cpp                                                      */
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
#include "gl_renderer.hpp"
#include "common/opengl.hpp"

#include "gl_shader.hpp"

#include "core/logger.hpp"
#include "core/filesys/filesys.hpp"

namespace sun
{

gl_shader* ugly_global_shader;

gl_renderer::gl_renderer() {}

gl_renderer::~gl_renderer() {}

void gl_renderer::init()
{
    renderer::init();

    auto error = glewInit();

    if (error != GLEW_NO_ERROR) {
        sun_logf_error("GLEW Initialization error: %s",
                glewGetErrorString(error));
        return;
    }

    auto [vertex_stage, frag_stage] = shader_utils::parse_source_pair(
        filesys::read_file("res/flat.glsl"));

    ugly_global_shader = new gl_shader(new gl_shader_stage(vertex_stage, shader_stage::type::vertex),
                          new gl_shader_stage(frag_stage, shader_stage::type::fragment));

    if (ugly_global_shader->build() != shader::status::ok) {
        sun_logf_error("%s", ugly_global_shader->get_warnings().c_str());
    } else {
        sun_log("Activating Shader");
        ugly_global_shader->attach();
    }

    sun_log_info("OpenGL Initialized");
}

void gl_renderer::shutdown()
{
    renderer::shutdown();
}

void gl_renderer::clear(const color& col)
{
    auto colf = to_colorf(col);
    glClearColor(colf.r, colf.g, colf.b, colf.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gl_renderer::clear()
{
    glClearColor(clear_color_.r,
                 clear_color_.g,
                 clear_color_.b,
                 clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}
