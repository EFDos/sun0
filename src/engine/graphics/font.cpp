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

Font::Font(Context& context)
:   Resource(context),
    library_(nullptr),
    face_(nullptr)
{
}

void Font::clear()
{
    Resource::clear();

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

bool Font::load(const std::string& filepath)
{
    clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft) != 0) {
        sun_log_error("Could not init FreeType Library");
        ft = nullptr;
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face) != 0) {
        sun_logf_error("Could no load Font %s", filepath.c_str());
        face = nullptr;
        return false;
    }

    library_ = ft;
    face_ = face;

    return Resource::load(filepath);
}

const Font::Glyph& Font::get_glyph(uint8 code, uint char_size) const
{
    if (library_ == nullptr || face_ == nullptr) {
        sun_log_warn("Retrieving glyph from unitialized Font");
    }

    GlyphTable& glyphes = pages_[char_size].glyphes;

    auto it = glyphes.find(code);

    if (it != glyphes.end()){
        return it->second;
    }
    else {
        Glyph g = load_glyph_(code, char_size);
        return glyphes.insert(std::make_pair(code, g)).first->second;
    }
}

float Font::get_kerning(uint32 first, uint32 second, uint char_size) const
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

float Font::get_line_spacing(uint char_size) const
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

const Texture* Font::get_page_texture(uint size) const
{
    return pages_[size].texture;
}

Font::Glyph Font::load_glyph_(uint8 code, uint char_size) const
{
    Glyph glyph;

    if (face_ == nullptr)
        return glyph;

    if (!set_current_size_(char_size))
        return glyph;

    FT_Face face = static_cast<FT_Face>(face_);

    FT_Int32 flags = FT_LOAD_TARGET_NORMAL | FT_LOAD_FORCE_AUTOHINT;
    if (FT_Load_Char(face, code, flags)){
        return glyph;
    }

    FT_Glyph glyph_desc;
    if (FT_Get_Glyph(face->glyph, &glyph_desc)){
        return glyph;
    }

    FT_Glyph_To_Bitmap(&glyph_desc, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_Bitmap& bitmap = reinterpret_cast<FT_BitmapGlyph>(glyph_desc)->bitmap;

    glyph.advance = static_cast<float>(face->glyph->metrics.horiAdvance)
                                       / static_cast<float>(1 << 6);

    int width  = bitmap.width;
    int height = bitmap.rows;

    if (width > 0 && height > 0)
    {
        const unsigned padding = 1;

        Page& page = pages_[char_size];

        glyph.tex_coords = find_glyph_rect_(page, width + 2 * padding,
                                            height + 2 * padding);

        glyph.tex_coords.x      += padding;
        glyph.tex_coords.y      += padding;
        glyph.tex_coords.w      -= 2 * padding;
        glyph.tex_coords.h      -= 2 * padding;

        glyph.rect = {
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

        page.texture->fill({(uint)glyph.tex_coords.x, (uint)glyph.tex_coords.y},
                           {(uint)glyph.tex_coords.w, (uint)glyph.tex_coords.h},
                           pixel_buffer_.data());
    }

    FT_Done_Glyph(glyph_desc);

    return glyph;
}

Recti Font::find_glyph_rect_(Page& page, uint width, uint height) const
{
    if (page.texture == nullptr) {
        page.texture = generate_page_texture_();
    }

    Row* row = nullptr;
    float best_ratio = 0.f;
    for (Row& it_row : page.rows)
    {
        float ratio = static_cast<float>(height) / it_row.height;

        if(ratio < 0.7f || ratio > 1.f)
            continue;

        if(width > page.texture->get_size().x - it_row.width)
            continue;

        if (ratio < best_ratio)
            continue;

        row = &it_row;
        best_ratio = ratio;
    }

    if (row == nullptr)
    {
        int row_height = height + height / 10;

        while (page.next_row + row_height >= page.texture->get_size().y ||
               width >= page.texture->get_size().x)
        {
            uint texture_width  = page.texture->get_size().w;
            uint texture_height = page.texture->get_size().h;

            uint tex_max_size = context_.get_system<Renderer>()->get_texture_max_size();

            if (texture_width * 2 <= tex_max_size &&
                texture_height * 2 <= tex_max_size)
            {
                //copy texture data to buffer, allocate bigger size and copy
                //data back to new texture
                const uint8* buffer;

                page.texture->map();
                buffer = page.texture->get_data();

                page.texture->clear();
                page.texture->resize({texture_width * 2,
                                        texture_height * 2});

                page.texture->fill({0,0}, {texture_width, texture_height},
                                     buffer);

                page.texture->unmap();
            }
            else {
                sun_log_error("Font: Failed to add new character: "
                              "Maximum texture size has been reached");
                return Recti(0, 0, 2, 2);
            }
        }
        page.rows.push_back(Row(page.next_row, row_height));
        page.next_row += row_height;
        row = &page.rows.back();
    }

    Recti glyph_rect(row->width, row->top, width, height);
    row->width += width;

    return glyph_rect;
}

bool Font::set_current_size_(uint char_size) const
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
                sun_logf_error("Font: Failed to set Font size to %s",
                          std::to_string(char_size).c_str());
            }
        }

        return result == FT_Err_Ok;
    }
    else {
        return true;
    }
}

Texture* Font::generate_page_texture_() const
{
    Texture* tex = context_.get_system<Renderer>()->create_texture();

    tex->set_usage(Texture::Usage::Dynamic);
    tex->set_format(Texture::Format::Rgba);
    tex->resize({128, 128});
    return tex;
}

}
