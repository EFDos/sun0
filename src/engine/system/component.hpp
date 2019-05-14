/*************************************************************************/
/*  component.hpp                                                        */
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

#include "common/object.hpp"

namespace sun {

class SUN_API component : public object
{
public:

    enum class property : uint8 {
        update          =   0x01,
        consume_event   =   0x02,
        draw            =   0x04
    };

    inline void set_update(bool update) {
        update ? flags_ |= property::update :
            flags_ &= ~property::update;
    }

    inline void set_consume_event(bool event) {
        event ? flags_ |= property::consume_event :
            flags_ &= ~property::consume_event;
    }

    inline void set_draw(bool draw) {
        draw ? flags_ |= property::draw : flags_ &= ~property::draw;
    }

    inline bool get_update() {
        return flags_ & property::update;
    }

    inline bool get_consume_event() {
        return flags_ & property::consume_event;
    }

    inline bool get_draw() {
        return flags_ & property::draw;
    }

    virtual const std::string& get_type_name() const = 0;

    virtual size_t get_type_hash() const = 0;

protected:

    component(context& p_context) : object(p_context), flags_(0) {}

    virtual ~component(context&) {}

    uint8   flags_;
};

#define SUN_COMPONENT_TYPE(type) const std::string& get_type_name() const override \
    { return get_static_type_name(); } \
    size_t get_type_hash() const override \
    { return get_static_type_hash(); } \
    static const std::string& get_static_type_name() { \
        static std::string static_name(#type); \
        return static_name; \
    } \
    static const size_t get_static_type_hash() \
    { return std::hash<std::string>{}(#type); } \

}
