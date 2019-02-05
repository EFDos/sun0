/*************************************************************************/
/*  renderer.cpp                                                         */
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
#include "renderer.hpp"
#include "core/logger.hpp"

namespace sun {

renderer::renderer()
{
}

renderer::~renderer()
{
}

void renderer::init()
{
    sun_log_info("Graphics System initialized.");
}

void renderer::shutdown()
{
    sun_log_info("Graphics System shutdown.");
}

void renderer::set_color(const color& col)
{
    clear_color_ = to_colorf(col);
}

}
