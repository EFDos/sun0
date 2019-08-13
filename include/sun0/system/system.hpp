/*************************************************************************/
/*  system.hpp                                                           */
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

#include <string>
#include <memory>

namespace sun {

class Context;
class Component;
class Resource;

class SUN_API System
{
public:

    explicit System(Context&);

    virtual ~System();

    virtual bool init();

    virtual void shutdown();

    virtual void update(float delta);

    template<typename T>
    T* create_component(uint id) {
        return static_cast<T*>(create_component_(T::get_static_type_hash(), id));
    }

    template<typename T>
    bool handles_component() {
        return handles_component_(T::get_static_type_hash());
    }

    template<typename T>
    T* create_resource() {
        return static_cast<T*>(create_resource_(T::get_static_type_hash()));
    }

    template<typename T>
    bool handles_resource() {
        return handles_resource_(T::get_static_type_hash());
    }

    virtual const std::string& get_type_name() const = 0;

    virtual size_t get_type_hash() const = 0;

protected:

    virtual Component* create_component_(uint type_hash, uint id) = 0;

    virtual bool handles_component_(uint type_hash) = 0;

    virtual Resource* create_resource_(uint type_hash);

    virtual bool handles_resource_(uint type_hash);

    Context&    context_;
    bool        initialized_;

};

#define SUN_SYSTEM_TYPE(type) const std::string& get_type_name() const override \
    { return get_static_type_name(); } \
    size_t get_type_hash() const override \
    { return get_static_type_hash(); } \
    static const std::string& get_static_type_name() { \
        static std::string static_name(#type); \
        return static_name; \
    } \
    static const size_t get_static_type_hash() \
    { static uint hash = std::hash<std::string>{}(#type); \
      return hash; \
    } \

}
