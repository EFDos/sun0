/*************************************************************************/
/*  clock.hpp                                                            */
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

#include "common/time.hpp"
#include <chrono>

namespace sun
{

class clock
{
public:

    clock();

    clock(const clock&) = delete;
    clock(const clock&&) = delete;
    clock& operator=(const clock&) = delete;
    clock& operator=(clock&&) = delete;

    void reset();

    time elapsed();

private:

    typedef std::chrono::high_resolution_clock high_res_clock;
    typedef std::chrono::duration<uint32, std::milli> milliseconds;

    std::chrono::time_point<high_res_clock> start_;
};

} // sun
