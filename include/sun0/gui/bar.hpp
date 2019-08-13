/*************************************************************************/
/*  bar.hpp                                                              */
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

#include "container.hpp"

namespace sun {

class SUN_API Bar : public Container
{
public:

    enum class Orientation
    {
        Top,
        Left,
        Right,
        Bottom
    };

    Bar(Context& context, Orientation orientation);

    void draw(Renderer*) const override;

    void handle_events(const Event&) override;

private:

    void on_parent_set_() override;

    Recti request_bounds(Recti&& bounds) override;

    Orientation orientation_;

};

}
