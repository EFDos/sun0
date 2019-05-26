/*************************************************************************/
/*  physics_server.hpp                                                   */
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

#include "physics_rasterizer.hpp"
#include "common/types.hpp"
#include "system/system.hpp"

#include <Box2D/Box2D.h>
#include <vector>

namespace sun {

class context;
class renderer;
class rigid_body;

namespace physics {

struct raycast_collision
{
    vector2f    point;
    vector2f    normal;
    rigid_body* owner = nullptr;
};

uint get_pixel_scale();

inline float scale_to_pixels(float val) {
    return val * get_pixel_scale();
}

inline float scale_to_meters(float val) {
    return val / get_pixel_scale();
}

float scale_to_meters(float val);

inline b2Vec2 to_b2vec(const vector2f& vec) {
    return b2Vec2(scale_to_meters(vec.x), -scale_to_meters(vec.y));
}

inline vector2f to_vec2(const b2Vec2& b2vec) {
    return {scale_to_pixels(b2vec.x), -scale_to_pixels(b2vec.y)};
}

}

class SUN_API physics_server final : public system
{
public:

    friend class rigid_body;

    SUN_SYSTEM_TYPE(SYS_PHYSICS_SERVER);

    explicit physics_server(context&);

    ~physics_server() = default;

    bool init() override;

    void shutdown() override;

    void update() override;

    void draw_physics_debug(renderer*);

    bool lazy_raycast(const vector2f& begin, const vector2f& end);

    physics::raycast_collision raycast(const vector2f& begin,
                                       const vector2f& end);

    void set_gravity(const vector2f& gravity);

    void set_pixel_scale(uint scale);

    void set_update_rate(float timestep, int vel_it, int pos_it);

    inline void set_debug_draw(bool b) {
        debug_draw_ = b;
    }

    static constexpr uint   default_meter = 64;

private:

    class lazy_raycast_callback : public b2RayCastCallback
    {
        bool is_colliding_;

        public:

            lazy_raycast_callback() : is_colliding_(false) {}

            float ReportFixture(b2Fixture*, const b2Vec2&, const b2Vec2&, float)
                override
            {
                is_colliding_ = true;
                return 0.f;
            }

            bool is_colliding() { return is_colliding_; }
    };

    class nearest_raycast_callback : public b2RayCastCallback
    {
        physics::raycast_collision&    collision_;

        public:

            nearest_raycast_callback(physics::raycast_collision& collision)
            : collision_(collision) {}

            float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                                const b2Vec2& normal, float fraction) override
            {
                collision_.point = physics::to_vec2(point);
                collision_.normal = physics::to_vec2(normal);
                collision_.owner    = static_cast<rigid_body*>
                                        (fixture->GetBody()->GetUserData());
                return fraction;
            }
    };

    inline b2World* get_b2_world() {
        return &world_;
    }

    b2World             world_;
    physics_rasterizer  debug_rasterizer_;
    //contact_listener
    float   timestep_;
    int     vel_iterations_;
    int     pos_iterations_;
    bool    debug_draw_;

    // system functions

    std::vector<rigid_body*> bodies_;

    component* create_component_(uint type_hash) override;

    bool handles_component_(uint type_hash) override;
};

} // sun
