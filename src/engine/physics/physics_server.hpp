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

#include "common/types.hpp"
#include "system/system.hpp"
#include "physics_rasterizer.hpp"
#include "contact_listener.hpp"

#include <Box2D/Box2D.h>

namespace sun {

class Context;
class Renderer;
class RigidBody;
class Raycast;

namespace physics {

struct RaycastCollision
{
    Vector2f    point;
    Vector2f    normal;
    RigidBody* owner = nullptr;
};

uint get_pixel_scale();

inline float scale_to_pixels(float val) {
    return val * get_pixel_scale();
}

inline float scale_to_meters(float val) {
    return val / get_pixel_scale();
}

float scale_to_meters(float val);

inline b2Vec2 to_b2vec(const Vector2f& vec) {
    return b2Vec2(scale_to_meters(vec.x), -scale_to_meters(vec.y));
}

inline Vector2f to_vec2(const b2Vec2& b2vec) {
    return {scale_to_pixels(b2vec.x), -scale_to_pixels(b2vec.y)};
}

}

class SUN_API PhysicsServer final : public System
{
public:

    friend class RigidBody;

    SUN_SYSTEM_TYPE(PhysicsServer);

    explicit PhysicsServer(Context&);

    ~PhysicsServer() = default;

    bool init() override;

    void shutdown() override;

    void update(float delta) override;

    void draw_physics_debug(Renderer*);

    bool lazy_raycast(const Vector2f& begin, const Vector2f& end);

    physics::RaycastCollision raycast(const Vector2f& begin,
                                       const Vector2f& end);

    void set_gravity(const Vector2f& gravity);

    void set_pixel_scale(uint scale);

    void set_update_rate(float timestep, int vel_it, int pos_it);

    inline void set_debug_draw(bool b) {
        debug_draw_ = b;
    }

    static constexpr uint   DEFAULT_METER = 64;

private:

    class LazyRaycastCallback : public b2RayCastCallback
    {
        bool is_colliding_;

        public:

            LazyRaycastCallback() : is_colliding_(false) {}

            float ReportFixture(b2Fixture*, const b2Vec2&, const b2Vec2&, float)
                override
            {
                is_colliding_ = true;
                return 0.f;
            }

            bool is_colliding() { return is_colliding_; }
    };

    class NearestRaycastCallback : public b2RayCastCallback
    {
        physics::RaycastCollision&    collision_;

        public:

            NearestRaycastCallback(physics::RaycastCollision& collision)
            : collision_(collision) {}

            float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                                const b2Vec2& normal, float fraction) override
            {
                collision_.point = physics::to_vec2(point);
                collision_.normal = physics::to_vec2(normal);
                collision_.owner    = static_cast<RigidBody*>
                                        (fixture->GetBody()->GetUserData());
                return fraction;
            }
    };

    inline b2World* get_b2_world() {
        return &world_;
    }

    b2World             world_;
    PhysicsRasterizer   debug_rasterizer_;
    ContactListener     contact_listener_;

    float   timestep_;
    int     vel_iterations_;
    int     pos_iterations_;
    bool    debug_draw_;

    // System functions

    std::vector<RigidBody*> bodies_;
    std::vector<Raycast*>   raycasts_;

    Component* create_component_(uint type_hash, uint id, bool init) override;

    bool handles_component_(uint type_hash) override;
};

} // sun
