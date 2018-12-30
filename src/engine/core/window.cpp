/*************************************************************************/
/*  window.cpp                                                           */
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
#include "common/opengl.hpp"
#include "window.hpp"
#include "event.hpp"
#include "logger.hpp"

#include <SDL2/SDL.h>
#include <cstring>

namespace sun {

window::window() : window_hndl_(nullptr), gl_context_(nullptr)
{
}

window::window(const std::string& name,
        const vector2u& size,
		bool fullscreen)
:   window()
{
	create(name, size, fullscreen);
}

window::~window()
{
	close();
}

void window::update()
{
	if (window_hndl_ == nullptr) {
        sun_log_warn("call ignored: can't update unitiliazed window");
        return;
    }
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(window_hndl_));
}

bool window::poll_event(event& e)
{
	if (window_hndl_ == nullptr) {
        sun_log_warn("call ignored: can't poll events on unitiliazed window");
        return false;
    }
	SDL_Event sdl_e;
    bool ret = SDL_PollEvent(&sdl_e);

    switch(sdl_e.type)
    {
    	case SDL_QUIT:
    	    e.type = event_type::closed;
    	    return ret;
    	case SDL_MOUSEWHEEL:
    		e.type = event_type::mouse_wheel_scrolled;
    		e.mouse_wheel_scroll.x = sdl_e.wheel.x;
    		e.mouse_wheel_scroll.y = sdl_e.wheel.y;
    		return ret;
    	case SDL_MOUSEBUTTONDOWN:
    		e.type = event_type::mouse_button_pressed;
    		e.mouse_button.button = mouse::button(sdl_e.button.button);
    		e.mouse_button.x = sdl_e.button.x;
    		e.mouse_button.y = sdl_e.button.y;
    		return ret;
    	case SDL_MOUSEBUTTONUP:
    		e.type = event_type::mouse_button_released;
    		e.mouse_button.button = mouse::button(sdl_e.button.button);
    		e.mouse_button.x = sdl_e.button.x;
    		e.mouse_button.y = sdl_e.button.y;
    		return ret;
    	case SDL_MOUSEMOTION:
    		e.type = event_type::mouse_moved;
    		e.mouse_move.x = sdl_e.motion.x;
    		e.mouse_move.y = sdl_e.motion.y;
    		e.mouse_move.x_rel = sdl_e.motion.xrel;
    		e.mouse_move.y_rel = sdl_e.motion.yrel;
    		return ret;
    	case SDL_KEYDOWN:
    		e.type = event_type::key_pressed;
    		e.key.code = keyboard::key(sdl_e.key.keysym.sym);
    		if (sdl_e.key.keysym.mod & KMOD_NONE) {
    			return ret;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_ALT) {
    			e.key.alt = true;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_SHIFT) {
    			e.key.shift = true;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_CTRL) {
    			e.key.control = true;
    		}
    		return ret;
    	case SDL_KEYUP:
    		e.type = event_type::key_released;
    		e.key.code = keyboard::key(sdl_e.key.keysym.sym);
    		if (sdl_e.key.keysym.mod & KMOD_NONE) {
    			return ret;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_ALT) {
    			e.key.alt = false;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_SHIFT) {
    			e.key.shift = false;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_CTRL) {
    			e.key.control = false;
    		}
    		return ret;
    	case SDL_TEXTINPUT:
    		e.type = event_type::text_entered;
    		std::memcpy(e.text_input.text, sdl_e.text.text, 32);
    		e.text_input.text_size = std::strlen(sdl_e.text.text);
    		return ret;
        default:
        	e.type = event_type::undefined;
            return ret;
    }
}

void window::create(const std::string& name,
                    const vector2u& size,
                    bool fullscreen)
{
	close();

	window_hndl_ = SDL_CreateWindow(name.c_str(),
	        SDL_WINDOWPOS_CENTERED,
	        SDL_WINDOWPOS_CENTERED,
	        size.w,
	        size.h,
	        fullscreen ? SDL_WINDOW_FULLSCREEN : 0
	                | SDL_WINDOW_OPENGL);

	if (window_hndl_ == nullptr) {
        sun_logf_error("could not create SDL Window: %s", SDL_GetError());
        return;
    } else {
        sun_log_info("SDL Window created");
    }

    gl_context_ = SDL_GL_CreateContext(static_cast<SDL_Window*>(window_hndl_));

    if (gl_context_ == nullptr) {
        sun_logf_error("could not create OpenGL Context: %s", SDL_GetError());
        return;
    } else {
        sun_log_info("OpenGL Context created");
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    #if defined(API_OPENGL_ES3)

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    #else

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    #endif

    SDL_GL_SetSwapInterval(true);

    auto error = glewInit();

    if (error != GLEW_NO_ERROR) {
        sun_logf_error("GLEW Initialization error: %s",
                glewGetErrorString(error));
        return;
    }

    sun_log_info("OpenGL Initialized");
}

void window::close()
{
    if (gl_context_ != nullptr) {
    	SDL_GL_DeleteContext(static_cast<SDL_GLContext>(gl_context_));
    	gl_context_ = nullptr;
    }
    if (window_hndl_ != nullptr) {
	    SDL_DestroyWindow(static_cast<SDL_Window*>(window_hndl_));
	    window_hndl_ = nullptr;
    }
}

}
