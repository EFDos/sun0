/*************************************************************************/
/*  config.hpp                                                           */
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

#if defined(_WIN32)

    #define SUN_PLATFORM_WINDOWS

#elif defined(__APPLE__) && defined(__MACH__)

    //TODO: test target as Mac or iOS
    #error Apple operating systems are not yet supported.

#elif defined(__unix__)

    #if defined(__ANDROID__)

        #define SUN_PLATFORM_ANDROID

    #elif defined(__linux__)

        #define SUN_PLATFORM_LINUX

    #elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)

        #define SUN_PLATFORM_BSD

    #else

        #error This Unix OS is not supported.
        #error You may contact developers and ask for it \
         or clone the project and DIY.

    #endif

#else

    #error This operating system is not supported.
    #error You may contact developers and ask for it \
     or clone the project and DIY.

#endif

#if !defined(NDEBUG)

    #define SUN_OUT_DEBUG

#endif

#ifdef SUN_PLATFORM_WINDOWS
    #ifdef SUN_BUILD_SHARED
        #define SUN_API __declspec(dllexport)
    #else
        #define SUN_API __declspec(dllimport)
    #endif
#else
    #define SUN_API
#endif
