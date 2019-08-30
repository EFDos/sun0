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
#include "scene/scene_manager.hpp"
#include "resources/resource_cache.hpp"
#include "physics/physics_server.hpp"
#include "graphics/opengl/renderer.hpp"
#include "audio/audio_server.hpp"
#include "script/script_context.hpp"
#include "gui/gui_system.hpp"

#include "logger.hpp"

namespace sun {

Context::Context()
{
}

Context::~Context()
{
}

void Context::init_systems()
{
    for (auto sys : systems_) {
        sys.second->init();
    }
}
void Context::shutdown_systems()
{
    for (auto sys : systems_) {
        sys.second->shutdown();
    }
}

void Context::clear_systems_components()
{
    for (auto sys : systems_) {
        sys.second->clear_components();
    }
}

System* Context::register_system_(const std::string& type)
{
    System* sys = nullptr;
    if (type.compare("Renderer") == 0) {
        sys = new opengl::Renderer(*this);
    }
    if (type.compare("AudioServer") == 0) {
        sys = new AudioServer(*this);
    }
    if (type.compare("ResourceCache") == 0) {
        sys = new ResourceCache(*this);
    }
    if (type.compare("PhysicsServer") == 0) {
        sys = new PhysicsServer(*this);
    }
    if (type.compare("GUISystem") == 0) {
        sys = new GUISystem(*this);
    }
    if (type.compare("ScriptContext") == 0) {
        sys = new ScriptContext(*this);
    }
    if (type.compare("SceneManager") == 0) {
        sys = new SceneManager(*this);
    }

    if (sys != nullptr) {
        sun_logf_info("Registered System: %s", type.c_str());
        systems_[sys->get_type_hash()] = sys;
    } else {
        sun_logf_error("Could not register system %s", type.c_str());
    }

    return sys;
}

System* Context::get_system_(size_t type_hash)
{
    auto it = systems_.find(type_hash);
    if (it != systems_.end()) {
        return it->second;
    }

    sun_log_debug("system not found");
    return nullptr;
}

}
