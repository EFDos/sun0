/*************************************************************************/
/*  text.cpp                                                             */
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
#include "text.hpp"

#include "core/logger.hpp"
#include "core/context.hpp"
#include "scene/entity.hpp"
#include "renderer.hpp"
#include "font.hpp"

#include "common/color.hpp"

namespace sun {

Text::Text(Context& context)
:   Drawable(context),
    color_(Color::WHITE),
    font_size_(16),
    vertices_(nullptr),
    indices_(nullptr),
    font_(nullptr)
{
    auto r = context_.get_system<Renderer>();
    vertices_ = r->create_vertex_buffer(sizeof(float) * 8, 0);
    indices_ = r->create_index_buffer(0);
}

void Text::init()
{
    Component::init();
}

void Text::update_bounding_rect()
{
    if (font_ == nullptr) {
        return;
    }

    float hspace = (float)font_->get_glyph(' ', font_size_).advance;
    float vspace = (float)font_->get_line_spacing(font_size_);
    float x = 0.f, y = font_size_;
    char prev_char = 0;
    Rectf last_glyph_rect;

    for (auto c : str_)
    {
        x += font_->get_kerning(prev_char, c, font_size_);
        prev_char = c;

        if(c == ' ' || c == '\n' || c == '\t')
        {
            switch(c)
            {
                case ' ': x += hspace; break;
                case '\t': x += hspace * 4; break;
                case '\n': y += vspace; x = 0.f; break;
            }
            continue;
        }

        const Font::Glyph& g = font_->get_glyph(c, font_size_);

        x += g.advance + g.rect.w / 6.f;

        last_glyph_rect = g.rect;
    }

    bounding_rect_.w = x + last_glyph_rect.x + last_glyph_rect.w;
    bounding_rect_.h = y + last_glyph_rect.y + last_glyph_rect.h;
}

void Text::update_geometry_()
{
    if (font_ == nullptr) {
        sun_log_warn("Text has no font defined");
        return;
    }

    for (auto c: str_) {
        font_->get_glyph(c, font_size_);
    }

    auto page_texture = font_->get_page_texture(font_size_);

    if (page_texture == nullptr) {
        return;
    }

    bounding_rect_.set_size(0, 0);

    vertices_->resize(str_.length() * 4);
    indices_->resize(str_.length() * 6);

    float hspace = (float)font_->get_glyph(' ', font_size_).advance;
    float vspace = (float)font_->get_line_spacing(font_size_);
    float x = 0.f, y = font_size_;
    int offset = 0;
    int i_offset = 0;
    uint i_value_offset = 0;
    char prev_char = 0;
    for (auto c : str_)
    {
        x += font_->get_kerning(prev_char, c, font_size_);
        prev_char = c;

        if(c == ' ' || c == '\n' || c == '\t')
        {
            switch(c)
            {
                case ' ': x += hspace; break;
                case '\t': x += hspace * 4; break;
                case '\n': y += vspace; x = 0.f; break;
            }
            continue;
        }

        const Font::Glyph& g = font_->get_glyph(c, font_size_);
        auto tex_size = page_texture->get_size();

        float pos_x = x + g.rect.x;
        float pos_y = y + g.rect.y;
        float pos_w = g.rect.w;
        float pos_h = g.rect.h;
        float tex_x = (float)g.tex_coords.x / (float)tex_size.x;
        float tex_y = (float)g.tex_coords.y / (float)tex_size.y;
        float tex_w = (float)g.tex_coords.w / (float)tex_size.x;
        float tex_h = (float)g.tex_coords.h / (float)tex_size.y;

        Colorf col = Color::to_colorf(color_);

        x += g.advance + g.rect.w / 6.f;

        float vertices[] = {
            pos_x,          pos_y,
            tex_x,          tex_y,
            col.r,          col.g,  col.b,   col.a,

            pos_x + pos_w,  pos_y,
            tex_x + tex_w,  tex_y,
            col.r,          col.g,  col.b,   col.a,

            pos_x + pos_w,  pos_y + pos_h,
            tex_x + tex_w,  tex_y + tex_h,
            col.r,          col.g,  col.b,   col.a,

            pos_x,          pos_y + pos_h,
            tex_x,          tex_y + tex_h,
            col.r,          col.g,  col.b,     col.a
        };

        bounding_rect_.w = pos_x + pos_w;
        bounding_rect_.h = pos_y + pos_h;

        uint32 quad_indices[] = {
            i_value_offset + 0, i_value_offset + 1, i_value_offset + 3,
            i_value_offset + 1, i_value_offset + 2, i_value_offset + 3
        };

        vertices_->fill_data(offset, 4, vertices);
        indices_->fill_data(i_offset, 6, quad_indices);

        offset += 4;
        i_offset += 6;
        i_value_offset += 4;
    }
}

void Text::draw(Renderer* renderer) const
{
    if (font_ == nullptr ||
        vertices_->get_vertex_count() == 0 ||
        indices_->get_index_count() == 0) {
        return;
    }
    if (font_->get_page_texture(font_size_) == nullptr) {
        sun_log_error("Font page texture is null");
        return;
    }
    if (owning_entity_ != nullptr) {
        renderer->set_model_transform(owning_entity_->get_global_transform());
    } else if (transform_ != nullptr) {
        renderer->set_model_transform(*transform_);
    }
    renderer->set_draw_mode(Renderer::DrawMode::Triangles);
    renderer->draw_indexed(*vertices_, *indices_,
        font_->get_page_texture(font_size_), nullptr);
}

}
