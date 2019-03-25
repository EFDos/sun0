/*************************************************************************/
/*  context.hpp                                                          */
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
#include "context.hpp"

#include "system/system.hpp"
#include "graphics/opengl/renderer.hpp"
#include "audio/audio_server.hpp"

#include "logger.hpp"

namespace sun {

context::context()
{
}

context::~context()
{
}

void context::init_systems()
{
    for (auto sys : systems_) {
        sys.second->init();
    }
}
void context::shutdown_systems()
{
    for (auto sys : systems_) {
        sys.second->shutdown();
    }
}

system* context::register_system_(const std::string& type)
{
    system* sys = nullptr;
    if (type == "SYS_RENDERER") {
        sun_logf_debug("registering new opengl renderer as %s", type.c_str());
        sys = new opengl::renderer(*this);
    }
    if (type == "SYS_AUDIO_SERVER") {
        sun_logf_debug("registering new openal renderer as %s", type.c_str());
        sys = new audio_server(*this);
    }

    if (sys != nullptr) {
        systems_[sys->get_type_hash()] = sys;
    } else {
        sun_logf_error("Could not register system %s", type.c_str());
    }

    return sys;
}

system* context::get_system_(size_t type_hash)
{
    auto it = systems_.find(type_hash);
    if (it != systems_.end()) {
        sun_logf_debug("returning system %s", it->second->get_type_name().c_str());
        return it->second;
    }

    sun_log_debug("system not found");
    return nullptr;
}

}
