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
#include "window.hpp"
#include "event.hpp"
#include "logger.hpp"

#include <SDL2/SDL.h>

namespace sun {

window::window() : window_hndl_(nullptr)
{
    //SDL_Init(SDL_INIT_VIDEO);
}

window::window(const std::string& name,
               int width, int height, bool fullscreen)
:   window()
{
	create(name, width, height, fullscreen);
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
	//SDL_UpdateWindow(window_hndl_);
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
        default:
            return ret;
    }
}

void window::create(const std::string& name,
                    int width, int height, bool fullscreen)
{
	window_hndl_ = SDL_CreateWindow(name.c_str(),
	        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	        width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

	if (window_hndl_ == nullptr) {
        sun_logf_error("could not create SDL Window: %s", SDL_GetError());
    } else {
        sun_log_info("SDL Window created");
    }
}

void window::close()
{
	if (window_hndl_ != nullptr) {
	    SDL_DestroyWindow(window_hndl_);
	    window_hndl_ = nullptr;
	}
}

}
