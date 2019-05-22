/*************************************************************************/
/*  resource_cache.cpp                                                   */
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
#include "resource_cache.hpp"
#include "core/logger.hpp"

namespace sun {

resource_cache::resource_cache(context& p_context)
: system(p_context)
{
}

bool resource_cache::init()
{
    sun_log_info("Resource Cache System ready.");
    return system::init();
}

void resource_cache::shutdown()
{
    system::shutdown();
    sun_log_info("Resource Cache Shutdown.");
}

}
