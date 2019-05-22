/*************************************************************************/
/*  application.hpp                                                      */
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
#include "application.hpp"
#include "../version.hpp"

#include "resources/resource_cache.hpp"
#include "graphics/opengl/renderer.hpp"
#include "audio/audio_server.hpp"

#include "logger.hpp"
#include "event.hpp"

namespace sun {

application::application(context& p_context)
:   context_(p_context),
    running_(false)
{
    sun_printf("******* Sun-0 Engine *******\n"
                 "Build: %s, %s",
                 version::string,
                 version::codename);
    sun_print("****************************");

	window_.create("Sandbox - Sun0 Engine", {1280, 720});

    context_.register_system<sun::resource_cache>();
    context_.register_system<sun::audio_server>();
    renderer_ = context_.register_system<sun::renderer>();

    context_.init_systems();
}

application::~application()
{
    context_.shutdown_systems();
}

int application::run()
{
	running_ = true;
    while(running_) {
    	event e;
    	if (window_.is_open()) {
            while (window_.poll_event(e)) {
    	        on_event(e);
            }
        }
        on_update();
        context_.update_systems();
        window_.update();
    }
    window_.close();
    return 0;
}

void application::on_event(event& e)
{
	if (e.type == event_type::closed) {
		running_ = false;
	}
}

} // sun
