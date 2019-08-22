/*************************************************************************/
/*  script.cpp                                                           */
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
#include "script.hpp"
#include "script_context.hpp"

namespace sun {

Script::Script(Context& context)
:   Component(context)
{}

void Script::handle_events(Event& event)
{
    if (hndl_ev_callback_ != nullptr) {
        hndl_ev_callback_(owning_entity_, event);
    }
}

void Script::update(float delta)
{
    if (dt_update_callback_ != nullptr) {
        dt_update_callback_(owning_entity_, delta);
    }
}

void Script::load(const std::string& filename)
{
    script_context_->register_script(this, filename);
    filename_ = filename;
}

void Script::reload()
{
    script_context_->register_script(this, filename_, true);
}

}
