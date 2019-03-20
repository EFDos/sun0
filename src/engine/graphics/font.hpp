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
#include <vector>

namespace sun
{

class SUN_API font
{
public:

    struct glyph
    {
        float       advance;
        rectf       rect;
        recti       tex_coords;

        glyph() : advance(0) {}
    };

    font();

    ~font();

    void load(const std::string& filepath);

    const glyph& get_glyph(uint8 code, uint char_size) const;

    float get_kerning(uint32 first, uint32 second, uint size) const;

    float get_line_spacing(uint size) const;

    const texture& get_page_texture(uint size) const;

private:

    struct row
    {
        uint    width;
        uint    height;
        uint    top;

        row(uint _top, uint _height) : width(0), height(_height), top(_top)
        {
            // nothing
        }
    };

    typedef std::unordered_map<uint8, glyph> glyph_table;

    struct page
    {
        glyph_table         glyphes;
        texture*            page_texture;
        uint                next_row;
        std::vector<row>    rows;

        page();
    };

    void cleanup_();

    glyph load_glyph_(uint8 code_point, uint char_size) const;

    recti find_glyph_rect_(page&, uint width, uint height) const;

    bool set_current_size_(uint char_size) const;

    mutable std::unordered_map<uint, page>  pages_;
    mutable std::vector<uint8>              pixel_buffer_;

    void*                           library_;
    void*                           face_;

};

} // sun
