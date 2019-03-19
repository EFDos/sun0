/*************************************************************************/
/*  font.cpp                                                             */
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
#include "font.hpp"

#include "renderer.hpp"

#include "texture.hpp"
#include "core/logger.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <algorithm>

namespace sun
{

font::font()
:   library_(nullptr),
    face_(nullptr)
{
}

font::~font()
{
    cleanup_();
}

void font::cleanup_()
{
    if (face_ == nullptr){
        FT_Done_Face(static_cast<FT_Face>(face_));
    }

    if (library_ == nullptr){
        FT_Done_FreeType(static_cast<FT_Library>(library_));
    }

    face_ = nullptr;
    library_ = nullptr;

}

void font::load(const std::string& filepath)
{
    cleanup_();

    FT_Library ft;
    if (FT_Init_FreeType(&ft) != 0) {
        sun_log_error("Could not init FreeType Library");
    }

    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face) != 0) {
        sun_logf_error("Could no load font %s", filepath.c_str());
    }

    library_ = ft;
    face_ = face;
}

const font::glyph& font::get_glyph(uint8 code, uint char_size) const
{
    auto page_it = pages_.find(char_size);

    if (page_it == pages_.end()) {
        pages_[char_size] = generate_page_(char_size);
    }

    auto& glyphes = pages_[char_size].glyphes;

    auto it = glyphes.find(code);

    if (it == glyphes.end()) {
        glyphes[code] = load_glyph_(code, char_size);
    }

    return glyphes[code];
}

float font::get_kerning(uint32 first, uint32 second, uint char_size) const
{
    if (first == 0 && second == 0){
        return 0.f;
    }

    FT_Face face = static_cast<FT_Face>(face_);

    if (face != nullptr && FT_HAS_KERNING(face) && set_current_size_(char_size))
    {
        FT_UInt index1 = FT_Get_Char_Index(face, first);
        FT_UInt index2 = FT_Get_Char_Index(face, second);

        FT_Vector kerning;
        FT_Get_Kerning(face, index1, index2, FT_KERNING_DEFAULT, &kerning);

        if (!FT_IS_SCALABLE(face)){
            return static_cast<float>(kerning.x);
        }

        return static_cast<float>(kerning.x) / static_cast<float>(1 << 6);
    }
    else {
        return 0.f;
    }
}

float font::get_line_spacing(uint char_size) const
{
    FT_Face face = static_cast<FT_Face>(face_);

    if (face != nullptr && set_current_size_(char_size))
    {
        return static_cast<float>(face->size->metrics.height) / static_cast<float>(1 << 6);
    }
    else {
        return 0.f;
    }
}

const texture* font::get_page_texture(uint size) const
{
    auto it = pages_.find(size);

    if (it == pages_.end()) {
        pages_[size] = generate_page_(size);
    }

    return pages_[size].tex;
}

font::page font::generate_page_(uint char_size) const
{
    page p;
    p.tex = system::get<renderer>("SYS_RENDERER")->create_texture();
    p.tex->set_format(texture::format::red);
    p.tex->resize({512, 512});
    return p;
}

font::glyph font::load_glyph_(uint8 code, uint char_size) const
{
    glyph g;

    if (face_ == nullptr || set_current_size_(char_size) == false) {
        sun_log_error("Error loading glyph");
        return g;
    }

    FT_Face ft_face = static_cast<FT_Face>(face_);
    if (FT_Load_Char(ft_face, code, FT_LOAD_RENDER) != 0)
    {
        sun_logf_error("Error loading character glyph \'%c\'", code);
        return g;
    }

    g.rect = {
        static_cast<float>(ft_face->glyph->bitmap_left),
        static_cast<float>(ft_face->glyph->bitmap_top),
        static_cast<float>(ft_face->glyph->bitmap.width),
        static_cast<float>(ft_face->glyph->bitmap.rows)
    };

    g.advance = ft_face->glyph->advance.x;

    page& p = pages_[char_size];

    p.max_height = g.rect.h > p.max_height ? g.rect.h : p.max_height;

    sun_printf("max height: %d", p.max_height);

    if (p.next_origin.x + g.rect.w > p.tex->get_size().x) {
        p.next_origin.x = 0;
        p.next_origin.y += p.max_height;
    }

    p.tex->fill(p.next_origin, {
                    static_cast<uint>(g.rect.w),
                    static_cast<uint>(g.rect.h)
                },
                ft_face->glyph->bitmap.buffer);

    p.next_origin.x += g.rect.w;

    return g;
}

bool font::set_current_size_(uint char_size) const
{
    FT_Face face = static_cast<FT_Face>(face_);

    FT_UShort current_size = face->size->metrics.x_ppem;

    if (current_size != char_size)
    {
        FT_Error result = FT_Set_Pixel_Sizes(face, 0, char_size);

        if (result == FT_Err_Invalid_Pixel_Size)
        {
            if (!FT_IS_SCALABLE(face))
            {
                sun_logf_error("font: Failed to set font size to %s",
                          std::to_string(char_size).c_str());
            }
        }

        return result == FT_Err_Ok;
    }
    else {
        return true;
    }
}

}
