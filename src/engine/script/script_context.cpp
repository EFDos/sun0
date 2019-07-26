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

#include "scene/entity.hpp"

namespace sun {

ScriptContext::ScriptContext(Context& context)
:   System(context)
{}

bool ScriptContext::init()
{
    lua_state_.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::string,
        sol::lib::bit32);;

    lua_state_.new_usertype<Entity>("Entity",
        "get_name", &Entity::get_name);

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

void ScriptContext::register_script(Script* script, const std::string& filename)
{
    auto it = script_registry_.find(filename);

    if (it == script_registry_.end())
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
        auto update_callback = lua_state_[filename + "_update"] =
            lua_state_["update"];

        if (update_callback != sol::nil)
        {
            script_reg.update_callback = update_callback;
            script->set_update_callback(update_callback);
            script_registry_[filename] = script_reg;
        } else {
            sun_logf_warn("Failed to load \"update\" function from script \"%s\"",
            filename.c_str());
            return;
        }

        return;
    }

    script->set_update_callback(it->second.update_callback);
}

Component* ScriptContext::create_component_(uint type_hash, uint id)
{
    if (type_hash == Script::get_static_type_hash()) {
        Script* script = new Script(context_);
        script->set_id(id);
        script->set_script_context(this);
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
