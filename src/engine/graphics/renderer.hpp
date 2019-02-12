/*************************************************************************/
/*  renderer.hpp                                                         */
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

namespace sun {

class SUN_API renderer : public system
{
public:

    SUN_SYSTEM_TYPE(renderer);

    struct info
    {
        std::string name;
        std::string version;
        std::string vendor;
        std::string device;
    };

    struct stats
    {
        int draw_calls;
        int canvas_switches;
        int shader_switches;
        int canvases;
    };

    renderer();

    virtual ~renderer();

    // implements system
    virtual void init() override;

    virtual void shutdown() override;

    virtual void clear(const color&) = 0;

    virtual void clear() = 0;

    virtual void set_color(const color&);

protected:

    colorf  clear_color_;
};

}
