/*************************************************************************/
/*  script_context.hpp                                                   */
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

#include "system/system.hpp"
#include "common/int.hpp"

#include <sol.hpp>
#include <string>

namespace sun {

class Entity;
class Script;

class SUN_API ScriptContext final : public System
{
public:

    SUN_SYSTEM_TYPE(ScriptContext)

    ScriptContext(Context&);

    bool init() override;

    void shutdown() override;

    void update(float delta) override;

    void register_script(Script* script, const std::string& filename);

    static void register_api(sol::state& state);

private:

    struct ScriptRegister
    {
        //std::string file;

        //std::function<void (sol::table&)>          init_callback;
        //std::function<void (sol::table&, event&)>  input_callback;
        std::function<void (Entity*, double)>    update_callback;
        //std::function<void (sol::table&,
        //                    std::string&&,
        //                    entity&,
        //                    sol::table&&)>     message_callback;
    };

    Component* create_component_(uint type_hash, uint id) override;

    bool handles_component_(uint type_hash) override;

    sol::state  lua_state_;

    std::vector<Script*>                            scripts_;
    std::unordered_map<std::string, ScriptRegister> script_registry_;
};

}
