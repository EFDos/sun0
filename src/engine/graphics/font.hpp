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
#include "resources/resource.hpp"
#include "texture.hpp"

#ifdef SUN_PLATFORM_HAIKU
#include <vector>
#include <unordered_map>
#endif

namespace sun
{

class Context;

class SUN_API Font : public Resource
{
public:

    SUN_RESOURCE_TYPE(Font)

    struct Glyph
    {
        float       advance;
        Rectf       rect;
        Recti       tex_coords;

        Glyph() : advance(0) {}
    };

    Font(Context&);

    bool load(const std::string& filepath) override;

    void clear() override;

    const Glyph& get_glyph(uint8 code, uint char_size) const;

    float get_kerning(uint32 first, uint32 second, uint size) const;

    float get_line_spacing(uint size) const;

    const Texture* get_page_texture(uint size) const;

private:

    struct Row
    {
        uint    width;
        uint    height;
        uint    top;

        Row(uint _top, uint _height) : width(0), height(_height), top(_top)
        {
            // nothing
        }
    };

    using GlyphTable = std::unordered_map<uint8, Glyph>;

    struct Page
    {
        Texture*            texture;
        GlyphTable          glyphes;
        uint                next_row;
        std::vector<Row>    rows;

        Page() : texture(nullptr), next_row(3) {}
    };

    void cleanup_();

    Glyph load_glyph_(uint8 code_point, uint char_size) const;

    Recti find_glyph_rect_(Page&, uint width, uint height) const;

    bool set_current_size_(uint char_size) const;

    Texture* generate_page_texture_() const;

    mutable std::unordered_map<uint, Page>  pages_;
    mutable std::vector<uint8>              pixel_buffer_;

    void*                           library_;
    void*                           face_;

};

} // sun
