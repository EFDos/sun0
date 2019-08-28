/*************************************************************************/
/*  script_context.cpp                                                   */
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
#include "script_context.hpp"
#include "script.hpp"
#include "core/logger.hpp"
#include "core/event.hpp"
#include "core/clock.hpp"

#include "scene/entity.hpp"

namespace sun {

ScriptContext::ScriptContext(Context& context)
:   System(context)
{}

bool ScriptContext::init()
{
    lua_state_.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::string,
        sol::lib::bit32);;

    register_api(lua_state_);

    sun_log_info("Lua State initialized.");
    return true;
}

void ScriptContext::shutdown()
{
    /*for (auto s : scripts_) {
        delete s;
    }
    scripts_.clear();*/
}

void ScriptContext::update(float delta)
{
    for (auto s : scripts_) {
        if (s->get_update()) {
            s->update(delta);
        }
    }
}

void ScriptContext::handle_events(Event& event)
{
    for (auto s : scripts_) {
        if (s->get_handle_event()) {
            s->handle_events(event);
        }
    }
}

void ScriptContext::hot_reload()
{
    sun_log_debug("ScriptContext: Performing Hot Reload...");
    Clock time;
    int i = 0;
    for (auto s : scripts_) {
        if (!s->is_terminal()) {
            s->reload();
            s->init();
            ++i;
        }
    }
    sun_logf_debug("ScriptContext: Reloaded %d scripts in %f seconds", i,
                   time.elapsed().as_seconds());
}

void ScriptContext::register_script(Script* script, const std::string& filename,
                                    bool reload)
{
    auto it = script_registry_.find(filename);

    if (it == script_registry_.end() || reload)
    {
        try {
            lua_state_.script_file(filename);
        } catch(const sol::error& e) {
            sun_logf_error("Failed to load script %s:\n%s", filename.c_str(),
                e.what());
                return;
        } catch(...) {
            sun_logf_error("Failed to load script \"%s\"", filename.c_str());
            return;
        }

        ScriptRegister  script_reg;

        auto init_callback = lua_state_[filename + "_init"] =
            lua_state_["init"];

        auto update_callback = lua_state_[filename + "_update"] =
            lua_state_["update"];

        auto event_callback = lua_state_[filename + "_handle_events"] =
            lua_state_["handle_events"];

        if (init_callback != sol::nil) {
            script_reg.init_callback = init_callback;
            script->init_callback_ = init_callback;
        }

        if (update_callback != sol::nil) {
            script_reg.update_callback = update_callback;
            script->update_callback_ = update_callback;
        }

        if (event_callback != sol::nil) {
            script_reg.event_callback = event_callback;
            script->event_callback_ = event_callback;
        }

        script_registry_[filename] = script_reg;
    } else {
        script->init_callback_ = it->second.init_callback;
        script->update_callback_ = it->second.update_callback;
        script->event_callback_ = it->second.event_callback;
    }
}

Component* ScriptContext::create_component_(uint type_hash, uint id, bool init)
{
    if (type_hash == Script::get_static_type_hash()) {
        Script* script = new Script(context_);
        if (init) {
            script->init();
        }
        script->set_id(id);
        scripts_.push_back(script);
        return static_cast<Component*>(script);
    }
    return nullptr;
}

bool ScriptContext::handles_component_(uint type_hash)
{
    if (type_hash == Script::get_static_type_hash()) {
        return true;
    }
    return false;
}

}
