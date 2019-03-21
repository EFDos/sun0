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

#include <string>

namespace sun {

class font;

class SUN_API text : public drawable
{
public:

    text(context& p_context);

    void draw(renderer*) const override;

    void set_text(const std::string&);

    inline void set_color(const color& c) {
        color_ = c;
    }

    inline void set_font(font* f) {
        font_ = f;
    }

    inline void set_character_size(uint size) {
        font_size_ = size;
    }

private:

    color           color_;
    uint            font_size_;
    vertex_buffer*  vertices_;
    index_buffer*   indices_;
    font*   font_;

};

}
