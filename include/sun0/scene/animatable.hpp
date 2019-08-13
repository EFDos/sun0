/*************************************************************************/
/*  animatable.hpp                                                       */
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

#include "common/config.hpp"
#include "common/variant.hpp"

#include <vector>

namespace sun {

class SUN_API Animatable
{
public:

    virtual ~Animatable() {}

    virtual void build_properties() {};

    inline void set_property(uint64 property_hash, Variant var)
    {
        for (size_t i = 0 ; i < properties_.size() ; ++i)
        {
            if (properties_[i] == property_hash) {
                set_property_(i, var);
                break;
            }
        }
    }

protected:

    virtual void set_property_(size_t property_idx, Variant var) {};

    std::vector<uint64>   properties_;
};

}
