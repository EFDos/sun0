/*************************************************************************/
/*  physics_rasterizer.hpp                                               */
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
#include "common/config.hpp"
#include <Box2D/Common/b2Draw.h>

namespace sun {

class Renderer;

class SUN_API PhysicsRasterizer : public b2Draw
{
public:

    PhysicsRasterizer(uint scale);

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
        override;

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
        override;

    void DrawPolygon(const b2Vec2* vertices, int32 count, const b2Color& color)
        override;

    void DrawSolidCircle(const b2Vec2& center, float32 radius,
        const b2Vec2& axis, const b2Color& color) override;

    void DrawSolidPolygon(const b2Vec2* vertices, int32 count,
        const b2Color& color) override;

    void DrawTransform(const b2Transform& transform) override;

    inline void set_renderer(Renderer* rend) {
        renderer_ = rend;
    }

    inline const Renderer* get_renderer() const {
        return renderer_;
    }

    inline void set_scale(uint scale) {
        scale_ = scale;
    }

    inline uint get_scale() const {
        return scale_;
    }

private:

    Renderer*   renderer_;
    uint        scale_;
};

}
