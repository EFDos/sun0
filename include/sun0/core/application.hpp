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
#pragma once
#include "core.hpp"
#include "context.hpp"
#include "main.hpp"
#include "window.hpp"
#include "common/time.hpp"

#include <string>

namespace sun {

class Event;
class SceneManager;
class GUISystem;
class Renderer;
class PhysicsServer;
class ScriptContext;

class SUN_API Application
{
public:

    explicit Application(Context&);

    virtual ~Application();

    virtual void on_update(float delta) = 0;

    virtual void on_event(Event& e);

    int run();

    inline void set_framerate(float framerate) {
        timestep_ = 1 / framerate;
    }

protected:

    Context&        context_;
	Window          window_;
	Renderer*       renderer_;
	SceneManager*   scene_manager_;
	GUISystem*      gui_;
	PhysicsServer*  physics_;
	ScriptContext*  script_context_;

private:

    bool    running_;
    float   timestep_;
};

}



#define SUN_DEFINE_MAIN_APP(classname) \
int run_application() \
{ \
    sun::Context runtime_context; \
    classname app(runtime_context); \
    sun_print("Running application " #classname ":\n"); \
    return app.run(); \
} \
SUN_DEFINE_MAIN(run_application());
