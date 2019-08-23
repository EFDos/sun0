/*************************************************************************/
/*  scene_tree.cpp                                                       */
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
#include "scene_tree.hpp"

namespace sun {

SceneTree::SceneTree(Context& context)
:   Object(context),
    root_(context),
    entity_count_(0)
{
    root_.set_scene(this);
}

void SceneTree::init()
{
    root_.init_components(true);
}

Entity* SceneTree::create_entity(const std::string& name)
{
    return root_.create_child(name);
}

void SceneTree::clear()
{
    root_.clear_children();
    root_.clear_components();
    entity_count_ = 0;
}

}
