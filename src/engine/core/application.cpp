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

#include "scene/scene_manager.hpp"
#include "resources/resource_cache.hpp"
#include "physics/physics_server.hpp"
#include "graphics/opengl/renderer.hpp"
#include "audio/audio_server.hpp"
#include "script/script_context.hpp"
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

	window_.create("Sun0", {1280, 720});

    context_.register_system<sun::ResourceCache>();
    context_.register_system<sun::AudioServer>();
    //gui_ = context_.register_system<sun::GUISystem>();
    physics_ = context_.register_system<sun::PhysicsServer>();
    renderer_ = context_.register_system<sun::Renderer>();
    scene_manager_ = context_.register_system<sun::SceneManager>();
    script_context_ = context_.register_system<sun::ScriptContext>();

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
        timestep = time - last_frame_time;
        last_frame_time = time;

    	if (window_.is_open()) {
    	    Event e;
            while (window_.poll_event(e)) {
    	        on_event(e);
    	        script_context_->handle_events(e);
            }
        }
        on_update(timestep);
        script_context_->update(timestep);
        scene_manager_->update(timestep);
        physics_->update(timestep);
        renderer_->update(timestep);

        renderer_->render();
        renderer_->set_model_transform(Matrix4());
        physics_->draw_physics_debug(renderer_);
        if (gui_ != nullptr) {
            gui_->render(renderer_);
        }

        #ifdef SUN_OUT_DEBUG
        renderer_->set_camera_transform(Matrix4());
        renderer_->draw_text(std::to_string(timestep));
        #endif

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

	#ifdef SUN_OUT_DEBUG
	if (e.type == EventType::KeyPressed) {
	    if (e.key.code == keyboard::Key::R && e.key.control) {
	        if (script_context_ != nullptr) {
	            script_context_->hot_reload();
	        }
	    }
	    if (e.key.code == keyboard::Key::D && e.key.control) {
	        if (physics_ != nullptr) {
	            physics_->toggle_debug_draw();
	        }
	    }
	}
	#endif

    if (gui_ != nullptr) {
        gui_->handle_events(e);
    }
}

} // sun
