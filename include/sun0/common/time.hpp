/*************************************************************************/
/*  time.hpp                                                             */
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

#include "int.hpp"
#include "config.hpp"

namespace sun {

class SUN_API Time
{
public:

    Time() : microseconds_(0) {}

    Time(int64 microseconds) : microseconds_(microseconds) {}

    inline float as_seconds() const {
        return microseconds_ / 1000000.f;
    }

    inline int32 as_milliseconds() const {
        return static_cast<int32>(microseconds_ / 1000);
    }

    inline int64 as_microseconds() const {
        return microseconds_;
    }

    inline static Time seconds(float amount) {
        return Time(static_cast<int64>(amount * 1000000));
    }

    inline static Time milliseconds(int32 amount) {
        return Time(static_cast<int64>(amount * 1000));
    }

    inline static Time microseconds(int64 amount) {
        return Time(amount);
    }

    bool operator!=(const Time& other) {
        return microseconds_ != other.microseconds_;
    }

    static Time ZERO;

private:

    int64   microseconds_;
};

}
