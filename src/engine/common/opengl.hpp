/*************************************************************************/
/*  opengl.hpp                                                           */
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

#include "config.hpp"

#if defined(SUN_PLATFORM_WINDOWS)

    #define API_OPENGL_3
    #include <GL/glew.h>

#elif defined(SUN_PLATFORM_OSX)

    #define API_OPENGL_3
    #include <OpenGL/glew.h>

#elif defined(SUN_PLATFORM_LINUX) || defined(SUN_PLATFORM_BSD) || \
        defined(SUN_PLATFORM_BEOS)

    #if defined(API_OPENGL_ES3)

        #define API_OPENGL_ES

        #pragma message("Building GLES3 Renderer")
        #include <GLES3/gl3.h>
        #include <GLES3/gl3ext.h>

    #elif defined(API_OPENGL_ES2)

        #define API_OPENGL_ES

        #define API_OPENGL_ES2
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>

    #else

        #define API_OPENGL_3
        #include <GL/glew.h>

    #endif

#elif defined(SYS_ANDROID)

    #define API_OPENGL_ES3
    #include <GLES/gl.h>
    #include <GLES/glext.h>

#elif defined(SYS_IOS)

    #define API_OPENGL_ES3
    #include <OpenGLES/ES1/gl.h>
    #include <OpenGLES/ES1/glext.h>

#endif

