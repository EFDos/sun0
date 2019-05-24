/*************************************************************************/
/*  physics_server.hpp                                                   */
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

#include <Box2D/Box2D.h>

namespace sun {

class context;

class SUN_API physics_server final : public system
{
public:

    SUN_SYSTEM_TYPE(SYS_PHYSICS_SERVER);

    explicit physics_server(context&);

    ~physics_server() = default;

    bool init() override;

    void shutdown() override;

private:

    b2World world_;

    // system functions

    component* create_component_(uint type_hash) override;

    bool handles_component_(uint type_hash) override;
};

}
