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
#include "clock.hpp"

#include "resources/resource_cache.hpp"
#include "physics/physics_server.hpp"
#include "graphics/opengl/renderer.hpp"
#include "audio/audio_server.hpp"
#include "gui/gui_system.hpp"

#include "logger.hpp"
#include "event.hpp"

namespace sun {

Application::Application(Context& context)
:   context_(context),
    gui_(nullptr),
    running_(false),
    timestep_(1 / 60.f)
{
    sun_printf("******* Sun-0 Engine *******\n"
                 "Build: %s, %s",
                 version::string,
                 version::codename);
    sun_print("****************************");

	window_.create("Sandbox - Sun0 Engine", {1280, 720});

    context_.register_system<sun::ResourceCache>();
    context_.register_system<sun::AudioServer>();
    //gui_ = context_.register_system<sun::GUISystem>();
    physics_ = context_.register_system<sun::PhysicsServer>();
    renderer_ = context_.register_system<sun::Renderer>();

    //physics_->set_debug_draw(true);

    context_.init_systems();
}

Application::~Application()
{
    context_.shutdown_systems();
}

int Application::run()
{
	running_ = true;
	float last_frame_time = 0.f;
	float timestep = 0.f;
	Clock timer;
    while(running_) {

        float time = timer.elapsed().as_seconds();
        timestep += time - last_frame_time;
        last_frame_time = time;

        if (timestep > timestep_)
        {
            Event e;
        	if (window_.is_open()) {
                while (window_.poll_event(e)) {
        	        on_event(e);
                }
            }
            on_update(timestep);
            physics_->update(timestep);
            renderer_->update(timestep);
            timestep = 0.f;
        }

        renderer_->render();
        renderer_->set_model_transform(Matrix4());
        physics_->draw_physics_debug(renderer_);
        if (gui_ != nullptr) {
            gui_->render(renderer_);
        }
        window_.update();

    }
    window_.close();
    return 0;
}

void Application::on_event(Event& e)
{
	if (e.type == EventType::Closed) {
		running_ = false;
	}
    if (gui_ != nullptr) {
        gui_->handle_events(e);
    }
}

} // sun
