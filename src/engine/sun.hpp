/*************************************************************************/
/*  sun.hpp                                                              */
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

// VERSION
#include "version.hpp"

// CORE & CONFIG
#include "common/types.hpp"
#include "common/opengl.hpp"
#include "core/filesys/filesys.hpp"
#include "core/logger.hpp"
#include "core/application.hpp"
#include "core/event.hpp"
#include "core/context.hpp"

// TYPES
#include "common/types.hpp"
#include "common/shapes/rectangle.hpp"
#include "common/shapes/circle.hpp"
#include "common/shapes/convex.hpp"

// RESOURCES
#include "resources/resource_cache.hpp"

// GRAPHICS
#include "graphics/image.hpp"
#include "graphics/font.hpp"
#include "graphics/camera.hpp"
#include "graphics/sprite.hpp"
#include "graphics/sprite_batch.hpp"
#include "graphics/text.hpp"
#include "graphics/shape2D.hpp"
#include "graphics/renderer.hpp"
#include "graphics/vertex_buffer.hpp"
#include "graphics/index_buffer.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"

// GUI
#include "gui/gui_system.hpp"
#include "gui/frame.hpp"

// AUDIO
#include "audio/sound_stream.hpp"

// PHYSICS
#include "physics/rigid_body.hpp"

//SCENE
#include "scene/scene_tree.hpp"

/*********** ENTRY POINT ***********/
#include "core/main.hpp"
/***********************************/
