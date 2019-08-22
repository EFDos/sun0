/*************************************************************************/
/*  scene_manager.hpp                                                    */
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

namespace sun {

class Animation;

class SUN_API SceneManager : public System
{
public:

    SUN_SYSTEM_TYPE(SceneManager)

    SceneManager(Context&);

    bool init() override;

    void shutdown() override;

    void update(float delta) override;

private:

    Component* create_component_(uint type_hash, uint id) override;

    bool handles_component_(uint type_hash) override;

    std::vector<Animation*> animations_;
};

}
