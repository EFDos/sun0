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
#include "main.hpp"

#include <string>

namespace sun {

class SUN_API application
{
public:

    explicit application();

    virtual ~application();

    virtual void update() = 0;

    int run();
};

}

#define SUN_DEFINE_MAIN_APP(classname) \
int run_application() \
{ \
    classname* application = new classname(); \
    sun_print("Running application " #classname "\n"); \
    return application->run(); \
} \
SUN_DEFINE_MAIN(run_application());
