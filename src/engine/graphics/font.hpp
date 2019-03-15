/*************************************************************************/
/*  font.hpp                                                             */
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
#include "texture.hpp"

#include <unordered_map>

namespace sun
{

class SUN_API font
{
public:

    struct glyph
    {
        float       advance;
        rectf       rect;

        glyph() : advance(0) {}
    };

    font();

    ~font();

    void load(const std::string& filepath);

    const glyph& get_glyph(uint8 code, uint char_size) const;

    float get_kerning(uint32 first, uint32 second, uint size) const;

    float get_line_spacing(uint size) const;

private:

    void cleanup_();

    glyph load_glyph_(uint8 code_point, uint char_size) const;

    bool set_current_size_(uint char_size) const;

    void*                           library_;
    void*                           face_;

    mutable std::unordered_map<char, glyph> glyphes_;
};

} // sun
