/*************************************************************************/
/*  Window.cpp                                                           */
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
#include "window.hpp"
#include "event.hpp"
#include "logger.hpp"

#include <SDL2/SDL.h>
#include <cstring>

namespace sun {

Window::Window() : window_hndl_(nullptr), gl_context_(nullptr)
{
}

Window::Window(const std::string& name,
        const Vector2u& size,
		bool fullscreen)
:   Window()
{
	create(name, size, fullscreen);
}

Window::~Window()
{
	close();
}

void Window::update()
{
	if (window_hndl_ == nullptr) {
        sun_log_warn("call ignored: can't update unitiliazed Window");
        return;
    }
    SDL_GL_SwapWindow(static_cast<SDL_Window*>(window_hndl_));
}

bool Window::poll_event(Event& e)
{
	if (window_hndl_ == nullptr) {
        sun_log_warn("call ignored: can't poll events on unitiliazed Window");
        return false;
    }
	SDL_Event sdl_e;
    bool ret = SDL_PollEvent(&sdl_e);

    switch(sdl_e.type)
    {
    	case SDL_QUIT:
    	    e.type = EventType::Closed;
    	    return ret;
    	case SDL_MOUSEWHEEL:
    		e.type = EventType::MouseWheelScrolled;
    		e.mouse_wheel_event.x = sdl_e.wheel.x;
    		e.mouse_wheel_event.y = sdl_e.wheel.y;
    		return ret;
    	case SDL_MOUSEBUTTONDOWN:
    		e.type = EventType::MouseButtonPressed;
    		e.mouse_button_event.button = mouse::Button(sdl_e.button.button);
    		e.mouse_button_event.x = sdl_e.button.x;
    		e.mouse_button_event.y = sdl_e.button.y;
    		return ret;
    	case SDL_MOUSEBUTTONUP:
    		e.type = EventType::MouseButtonReleased;
    		e.mouse_button_event.button = mouse::Button(sdl_e.button.button);
    		e.mouse_button_event.x = sdl_e.button.x;
    		e.mouse_button_event.y = sdl_e.button.y;
    		return ret;
    	case SDL_MOUSEMOTION:
    		e.type = EventType::MouseMoved;
    		e.mouse_move_event.x = sdl_e.motion.x;
    		e.mouse_move_event.y = sdl_e.motion.y;
    		e.mouse_move_event.x_rel = sdl_e.motion.xrel;
    		e.mouse_move_event.y_rel = sdl_e.motion.yrel;
    		return ret;
    	case SDL_KEYDOWN:
    		e.type = EventType::KeyPressed;
    		e.key_event.code = keyboard::Key(sdl_e.key.keysym.sym);
    		if (sdl_e.key.keysym.mod & KMOD_NONE) {
    			return ret;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_ALT) {
    			e.key_event.alt = true;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_SHIFT) {
    			e.key_event.shift = true;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_CTRL) {
    			e.key_event.control = true;
    		}
    		return ret;
    	case SDL_KEYUP:
    		e.type = EventType::KeyReleased;
    		e.key_event.code = keyboard::Key(sdl_e.key.keysym.sym);
    		if (sdl_e.key.keysym.mod & KMOD_NONE) {
    			return ret;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_ALT) {
    			e.key_event.alt = false;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_SHIFT) {
    			e.key_event.shift = false;
    		}
    		if (sdl_e.key.keysym.mod & KMOD_CTRL) {
    			e.key_event.control = false;
    		}
    		return ret;
    	case SDL_TEXTINPUT:
    		e.type = EventType::TextEntered;
    		std::memcpy(e.text_input_event.text, sdl_e.text.text, 32);
    		e.text_input_event.text_size = std::strlen(sdl_e.text.text);
    		return ret;
        default:
        	e.type = EventType::Undefined;
        	//sun_logf_warn("Event type %d is undefined: ", sdl_e.type);
            return ret;
    }
}

void Window::create(const std::string& name,
                    const Vector2u& size,
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
}

void Window::close()
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

void Window::set_title(const std::string& title)
{
    if (window_hndl_ == nullptr) {
        sun_log_error("Can't set title on closed Window.");
        return;
    }
    SDL_SetWindowTitle(static_cast<SDL_Window*>(window_hndl_), title.c_str());
}

void Window::set_fullscreen(bool fullscreen)
{
    if (window_hndl_ == nullptr) {
        sun_log_error("Can't set closed Window to fullscreen.");
        return;
    }

    SDL_SetWindowFullscreen(static_cast<SDL_Window*>(window_hndl_),
        fullscreen ? SDL_WINDOW_FULLSCREEN : 0 | SDL_WINDOW_OPENGL);
}

void Window::set_vsync(bool vsync)
{
    if (window_hndl_ == nullptr) {
        sun_log_error("Can't set vsync on closed Window.");
        return;
    }

    if (!vsync) {
        SDL_GL_SetSwapInterval(0);
    } else {
        // Try adaptive vsync (Will fail if system doesn't support it)
        if (SDL_GL_SetSwapInterval(-1) == -1) {

            // We fall here if it fails and try vertical retrace mode
            SDL_GL_SetSwapInterval(1);
        }
    }
}

void Window::set_visible_cursor(bool visible)
{
    SDL_ShowCursor(visible);
}

void Window::set_size(const Vector2i& size)
{
    if (window_hndl_ == nullptr) {
        sun_log_error("Can't set size on closed Window.");
        return;
    }
    SDL_SetWindowSize(static_cast<SDL_Window*>(window_hndl_), size.x, size.y);
}

void Window::set_size(int x, int y)
{
    if (window_hndl_ == nullptr) {
        sun_log_error("Can't set size on closed Window.");
        return;
    }
    SDL_SetWindowSize(static_cast<SDL_Window*>(window_hndl_), x, y);
}

Vector2i Window::get_size() const
{
    if (window_hndl_ == nullptr) {
        sun_log_warn("Size of closed Window will be {0,0}.");
        return {0, 0};
    }

    Vector2i size;
    SDL_GetWindowSize(static_cast<SDL_Window*>(window_hndl_), &size.x, &size.y);

    return size;
}

}
