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

#include "core/context.hpp"
#include "core/logger.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <algorithm>

namespace sun
{

font::font(context& p_context)
:   object(p_context),
    library_(nullptr),
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

    pages_.clear();
    pixel_buffer_.clear();
}

void font::load(const std::string& filepath)
{
    cleanup_();

    FT_Library ft;
    if (FT_Init_FreeType(&ft) != 0) {
        sun_log_error("Could not init FreeType Library");
        ft = nullptr;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face) != 0) {
        sun_logf_error("Could no load font %s", filepath.c_str());
        face = nullptr;
        return;
    }

    library_ = ft;
    face_ = face;
}

const font::glyph& font::get_glyph(uint8 code, uint char_size) const
{
    if (library_ == nullptr || face_ == nullptr) {
        sun_log_warn("Retrieving glyph from unitialized font");
    }

    glyph_table& glyphes = pages_[char_size].glyphes;

    auto it = glyphes.find(code);

    if (it != glyphes.end()){
        return it->second;
    }
    else {
        glyph g = load_glyph_(code, char_size);
        return glyphes.insert(std::make_pair(code, g)).first->second;
    }
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
    return pages_[size].page_texture;
}

font::glyph font::load_glyph_(uint8 code, uint char_size) const
{
    glyph g;

    if (face_ == nullptr)
        return g;

    if (!set_current_size_(char_size))
        return g;

    FT_Face face = static_cast<FT_Face>(face_);

    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (FT_Load_Char(face, code, flags)){
        return g;
    }

    FT_Glyph glyph_desc;
    if (FT_Get_Glyph(face->glyph, &glyph_desc)){
        return g;
    }

    FT_Glyph_To_Bitmap(&glyph_desc, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyph_desc)->bitmap;

    g.advance = static_cast<float>(face->glyph->metrics.horiAdvance)
                                   / static_cast<float>(1 << 6);

    int width  = bitmap.width;
    int height = bitmap.rows;

    if (width > 0 && height > 0)
    {
        const unsigned padding = 1;

        page& p = pages_[char_size];

        g.tex_coords = find_glyph_rect_(p, width + 2 * padding,
                                        height + 2 * padding);

        g.tex_coords.x      += padding;
        g.tex_coords.y      += padding;
        g.tex_coords.w      -= 2 * padding;
        g.tex_coords.h      -= 2 * padding;

        g.rect = {
            static_cast<float>(face->glyph->metrics.horiBearingX)
            / static_cast<float>(1 << 6),

            -static_cast<float>(face->glyph->metrics.horiBearingY)
            / static_cast<float>(1 << 6),

            static_cast<float>(face->glyph->metrics.width)
            / static_cast<float>(1 << 6),

            static_cast<float>(face->glyph->metrics.height)
                / static_cast<float>(1 << 6)
        };

        pixel_buffer_.resize(width * height * 4, 255);
        const uint8* pixels = bitmap.buffer;
        if (bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
        {
            // Pixels are 1 bit monochrome values
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    size_t index = (x + y * width) * 4 + 3;
                    pixel_buffer_[index] =
                        ((pixels[x / 8]) & (1 << (7 - (x % 8)))) ? 255 : 0;
                }
                pixels += bitmap.pitch;
            }
        }
        else
        {
            // Pixels are 8 bits gray levels
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    // The color channels remain white, just fill the alpha channel
                    size_t index = (x + y * width) * 4 + 3;
                    pixel_buffer_[index] = pixels[x];
                }
                pixels += bitmap.pitch;
            }
        }

        p.page_texture->fill({(uint)g.tex_coords.x, (uint)g.tex_coords.y},
                             {(uint)g.tex_coords.w, (uint)g.tex_coords.h},
                             pixel_buffer_.data());
    }

    FT_Done_Glyph(glyph_desc);

    return g;
}

recti font::find_glyph_rect_(page& p, uint width, uint height) const
{
    if (p.page_texture == nullptr) {
        p.page_texture = generate_page_texture_();
    }

    row* r = nullptr;
    float best_ratio = 0.f;
    for (row& it_row : p.rows)
    {
        float ratio = static_cast<float>(height) / it_row.height;

        if(ratio < 0.7f || ratio > 1.f)
            continue;

        if(width > p.page_texture->get_size().x - it_row.width)
            continue;

        if (ratio < best_ratio)
            continue;

        r = &it_row;
        best_ratio = ratio;
    }

    if (r == nullptr)
    {
        int row_height = height + height / 10;

        while (p.next_row + row_height >= p.page_texture->get_size().y ||
               width >= p.page_texture->get_size().x)
        {
            uint texture_width  = p.page_texture->get_size().w;
            uint texture_height = p.page_texture->get_size().h;

            uint tex_max_size = context_.get_system<renderer>()->get_texture_max_size();

            if (texture_width * 2 <= tex_max_size &&
                texture_height * 2 <= tex_max_size)
            {
                //copy texture data to buffer, allocate bigger size and copy
                //data back to new texture
                const uint8* buffer;

                p.page_texture->map();
                buffer = p.page_texture->get_data();

                p.page_texture->clear();
                p.page_texture->resize({texture_width * 2,
                                        texture_height * 2});

                p.page_texture->fill({0,0}, {texture_width, texture_height},
                                     buffer);

                p.page_texture->unmap();
            }
            else {
                sun_log_error("Font: Failed to add new character: "
                              "Maximum texture size has been reached");
                return recti(0, 0, 2, 2);
            }
        }
        p.rows.push_back(row(p.next_row, row_height));
        p.next_row += row_height;
        r = &p.rows.back();
    }

    recti glyph_rect(r->width, r->top, width, height);
    r->width += width;

    return glyph_rect;
}

bool font::set_current_size_(uint char_size) const
{
    if (face_ == nullptr) {
        return false;
    }

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

texture* font::generate_page_texture_() const
{
    texture* tex = context_.get_system<renderer>()->create_texture();

    tex->set_format(texture::format::rgba);
    tex->resize({128, 128});
    return tex;
}

}
