/*************************************************************************/
/*  window.hpp                                                           */
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

#include "common/config.hpp"
#include "math/vector2.hpp"

#include <string>

namespace sun {

class Event;

class SUN_API Window
{
public:

	Window();

    Window(const std::string& name,
       	   const Vector2u& size,
    	   bool fullscreen = false);

	Window(const Window&) = delete;

	Window& operator=(const Window&) = delete;

    ~Window();

	void update();

    bool poll_event(Event& e);

    void create(const std::string& name,
    		    const Vector2u& size,
    		    bool fullscreen = false);

    void close();

	inline bool is_open() const { return window_hndl_ != nullptr; }

    void set_title(const std::string& title);

private:

    void*   window_hndl_;
    void*   gl_context_;
};

}
