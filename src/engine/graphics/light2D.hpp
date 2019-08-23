/*************************************************************************/
/*  light2D.hpp                                                          */
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

#include "system/component.hpp"

namespace sun {

class Renderer;

class SUN_API Light2D final : public Component
{
public:

    SUN_COMPONENT_TYPE(Light2D)

    Light2D(Context&);

    void init() override;

    void update(float delta) override;

    inline void set_intensity(float intensity) {
        intensity_ = intensity;
        update_intensity_();
    }

    inline void set_color(const Color& color) {
        color_ = color;
        update_color_();
    }

    inline float get_intensity() const {
        return intensity_;
    }

    inline const Color& get_color() const {
        return color_;
    }

private:

    void update_color_();

    void update_intensity_();

    void update_position_();

    Vector2f    pos_;
    int         rend_color_id_;
    float       intensity_;
    Color       color_;

    Renderer*   renderer_;
};

}
