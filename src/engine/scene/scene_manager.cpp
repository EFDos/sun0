/*************************************************************************/
/*  scene_manager.cpp                                                    */
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
#include "scene_manager.hpp"
#include "animation.hpp"

namespace sun {

SceneManager::SceneManager(Context& context)
:   System(context)
{}

bool SceneManager::init()
{
    return System::init();
}

void SceneManager::shutdown()
{
    System::shutdown();
}

void SceneManager::update(float delta)
{
    for (auto comp : components_) {
        if (comp->get_update()) {
            comp->update(delta);
        }
    }
}

Ref<Component> SceneManager::create_component_(uint type_hash, uint id, bool init)
{
    if (type_hash == Animation::get_static_type_hash()) {
        components_.push_back(std::make_shared<Animation>(context_));
        auto comp = components_.back();
        if (init) {
            comp->init();
        }
        return comp;
    }
    return nullptr;
}

bool SceneManager::handles_component_(uint type_hash)
{
    if (type_hash == Animation::get_static_type_hash()) {
        return true;
    }
    return false;
}

}
