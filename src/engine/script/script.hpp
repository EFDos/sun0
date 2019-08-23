/*************************************************************************/
/*  script.hpp                                                           */
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

#include "system/component.hpp"
#include <functional>

namespace sun {

class ScriptContext;

class SUN_API Script final : public Component
{
public:

    SUN_COMPONENT_TYPE(Script)

    Script(Context&);

    void init() override;

    void handle_events(Event&) override;

    void update(float delta) override;

    void load(const std::string& filename);

    void reload();

private:

    friend class ScriptContext;

    using UpdateCallback = std::function<void (Entity*, double)>;
    using EventCallback = std::function<void (Entity*, Event&)>;
    using InitCallback = std::function<void (Entity*)>;

    ScriptContext*  script_context_;

    InitCallback    init_callback_;
    UpdateCallback  update_callback_;
    EventCallback   event_callback_;

    std::string     filename_;
};

}
