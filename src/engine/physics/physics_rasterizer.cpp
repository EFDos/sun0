/*************************************************************************/
/*  physics_rasterizer.cpp                                               */
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
#include "physics_rasterizer.hpp"
#include "common/shapes/circle.hpp"
#include "graphics/renderer.hpp"
#include "core/logger.hpp"

#include <vector>

namespace sun {

physics_rasterizer::physics_rasterizer(uint scale)
:   scale_(scale)
{
}

void physics_rasterizer::DrawCircle(const b2Vec2& center, float32 radius,
    const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    vector2f vertices[30];

    shapes::circle circle;
    circle.set_radius(radius * scale_);
    circle.set_point_count(30);

    for(int i = 0 ; i < 30 ; ++i)
    {
        const vector2f& point = circle.get_point(i);
        vertices[i].x = point.x + (center.x - radius) * scale_;
        vertices[i].y = point.y + (center.y + radius) * scale_ * -1;
    }

    renderer_->draw_polygon(30, vertices, color::white);
}

void physics_rasterizer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2,
    const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    vector2f line [] = {
        {p1.x * scale_, p1.y * scale_ * -1},
        {p2.x * scale_, p2.y * scale_ * -1}
    };

    renderer_->draw_line(line[0], line[1], color::white);
}

void physics_rasterizer::DrawPolygon(const b2Vec2* vertices, int32 count,
    const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    std::vector<vector2f> nxt_vertices(count);

    for(int i = 0 ; i < count ; ++i)
    {
        nxt_vertices[i].x = vertices[i].x * scale_;
        nxt_vertices[i].y = vertices[i].y * scale_ * -1;
    }

    renderer_->draw_polygon(count, nxt_vertices.data(), color::white);
}

void physics_rasterizer::DrawSolidCircle(const b2Vec2& center, float32 radius,
    const b2Vec2& axis, const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    vector2f vertices[30];

    shapes::circle circle;
    circle.set_radius(radius * scale_);
    circle.set_point_count(30);

    for(int i = 0 ; i < 30 ; ++i)
    {
        const vector2f& point = circle.get_point(i);
        vertices[i].x = point.x + (center.x - radius) * scale_;
        vertices[i].y = point.y + (center.y + radius) * scale_ * -1;
    }

    vector2f line [2] = {
        {center.x * scale_, center.y * scale_ * -1},
        {axis.x * scale_, axis.y * scale_ * -1}
    };

    renderer_->draw_polygon(30, vertices, {255, 255, 255, (uint8)(col.r * 255)});
    renderer_->draw_line(line[0], line[1], color::white);
}

void physics_rasterizer::DrawSolidPolygon(const b2Vec2* vertices, int32 count,
    const b2Color& col)
{
    if(renderer_ == nullptr)
        return;

    std::vector<vector2f> nxt_vertices(count);

    for(int i = 0 ; i < count ; ++i)
    {
        nxt_vertices[i].x = vertices[i].x * scale_;
        nxt_vertices[i].y = vertices[i].y * scale_ * -1;
    }

    renderer_->draw_polygon(count, nxt_vertices.data(), {255, 255, 255,
        (uint8)(col.r * 255)});
}

void physics_rasterizer::DrawTransform(const b2Transform& transform)
{
    if(renderer_ == nullptr)
        return;

    b2Vec2 p1 = transform.p;
    b2Vec2 p2 = p1 + 0.5f * transform.q.GetXAxis();

    vector2f line_x[2] = {
        {p1.x * scale_, p1.y * scale_ * -1},
        {p2.x * scale_, p2.y * scale_ * -1}
    };

    p2 = p1 - 0.5f * transform.q.GetYAxis();

    vector2f line_y[2] = {
        {p1.x * scale_, p1.y * scale_ * -1},
        {p2.x * scale_, p2.y * scale_ * -1}
    };

    renderer_->draw_line(line_x[0], line_x[1], color::red);
    renderer_->draw_line(line_y[0], line_y[1], color::green);
}

}
