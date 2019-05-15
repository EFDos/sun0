/*************************************************************************/
/*  drawable.hpp                                                         */
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

#include "system/component.hpp"

namespace sun {

class renderer;

class SUN_API drawable : public component
{
public:

    drawable(context& p_context) : component(p_context) {}

    virtual ~drawable() {}

    virtual void draw(renderer*) const = 0;

    inline void set_bounding_rect(const recti& r) { bounding_rect_ = r; }

    const recti& get_bounding_rect() { return bounding_rect_; }

protected:

    recti   bounding_rect_;

};

}
