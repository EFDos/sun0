/*************************************************************************/
/*  text.hpp                                                             */
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
#include "common/types.hpp"

#include "drawable.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

namespace sun {

class Font;

class SUN_API Text final : public Drawable
{
public:

    SUN_COMPONENT_TYPE(Text)

    Text(Context& context);

    void init() override;

    void draw(Renderer*) override;

    void set_text(const std::string& p_str) {
        if (p_str != str_) {
            str_ = p_str;
            dirty_ = true;
        }
    }

    inline void set_color(const Color& c) {
        color_ = c;
        dirty_ = true;
    }

    inline void set_font(Font* f) {
        font_ = f;
        dirty_ = true;
    }

    inline void set_character_size(uint size) {
        font_size_ = size;
        dirty_ = true;
    }

private:

    void update_geometry_() override;

    std::string     str_;
    Color           color_;
    uint            font_size_;
    VertexBuffer*   vertices_;
    IndexBuffer*    indices_;
    Font*           font_;

};

}
