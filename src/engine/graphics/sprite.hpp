/*************************************************************************/
/*  sprite.hpp                                                           */
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

#include "drawable.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace sun {

class Texture;

class SUN_API Sprite final : public Drawable
{
public:

    SUN_COMPONENT_TYPE(Sprite)

    Sprite(Context& p_context);

    ~Sprite();

    // from Animatable
    void build_properties() override;

    void draw(Renderer* renderer) const override;

    void set_texture(const Texture* tex);

    void set_color(const Color& color);

    void set_frames(uint h_frames, uint v_frames);

    void set_frame(uint frame);

    inline void set_rect(const Rectu& r) {
        rect_ = r;
        update_geometry_();
    }

private:

    void set_property_(size_t property_idx, Variant var) override;

    void update_geometry_() override;

    Rectu           rect_;
    Colorf          color_;
    uint            v_frames_;
    uint            h_frames_;
    uint            frame_;

    VertexBuffer*  vertices_;
    IndexBuffer*   indices_;
    const Texture*  texture_;
};

}
