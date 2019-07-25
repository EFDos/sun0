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

namespace sun {

ScriptContext::ScriptContext(Context& context)
:   System(context)
{}

bool ScriptContext::init()
{
    lua_state_.open_libraries(sol::lib::base, sol::lib::jit, sol::lib::string,
        sol::lib::bit32);

    sun_log_info("Lua State initialized.");
    return true;
}

void ScriptContext::shutdown()
{
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
