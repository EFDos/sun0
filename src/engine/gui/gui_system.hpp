/*************************************************************************/
/*  gui_system.hpp                                                       */
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

#include "system/system.hpp"
#include "common/color.hpp"
#include "graphics/font.hpp"
#include "frame.hpp"

#include <memory>

namespace sun {

class Renderer;
class Event;

class SUN_API GUISystem : public System
{
public:

    struct Theme {
        Color  main_color;
        Color  accent_color;
        std::shared_ptr<Font>   font;

        Theme(const Color& p_main, const Color& p_accent)
        :   main_color(p_main), accent_color(p_accent), font(nullptr) {}
    };

    SUN_SYSTEM_TYPE(GUISystem)

    GUISystem(Context&);

    bool init() override;

    void shutdown() override;

    void render(Renderer*);

    void handle_events(const Event& e);

    void add_widget(Widget*);

    inline const Theme& get_default_theme() const {
        return default_theme_;
    }

private:

    virtual Component* create_component_(uint type_hash, uint id, bool init) override;

    virtual bool handles_component_(uint type_hash) override;

    Theme default_theme_;

    // Root widget is this frame
    Frame   frame_;
};

}
