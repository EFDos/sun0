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

#include "common/int.hpp"
#include "common/object.hpp"
#include "scene/animatable.hpp"

namespace sun {

class Entity;
class Event;

class SUN_API Component : public Object, public Animatable
{
public:

    enum class Property {
        Initialized     =   0x01,
        Update          =   0x02,
        HandleEvent     =   0x04,
        Draw            =   0x08,
        Terminal        =   0x10
    };

    virtual void init();

    virtual void handle_events(Event&);

    virtual void update(float delta);

    void queue_delete();

    void set_update(bool update);

    void set_handle_event(bool event);

    void set_draw(bool draw);

    inline void set_owning_entity(Entity* ent) {
        owning_entity_ = ent;
    }

    inline void set_id(uint id) {
        id_ = id;
    }

    bool get_update() const;

    bool get_handle_event() const;

    bool get_draw() const;

    bool is_terminal() const;

    bool is_initialized() const;

    inline uint8 get_property_mask() {
        return flags_;
    }

    inline Entity* get_owning_entity() const {
        return owning_entity_;
    }

    inline uint get_id() const {
        return id_;
    }

    virtual const std::string& get_type_name() const = 0;

    virtual size_t get_type_hash() const = 0;

protected:

    Component(Context& context);

    virtual ~Component() {}

    uint    id_;
    uint8   flags_;
    Entity* owning_entity_;
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
    { static uint hash = std::hash<std::string>{}(#type); \
      return hash; \
    } \

} // sun0
