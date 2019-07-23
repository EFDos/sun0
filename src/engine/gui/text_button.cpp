/*************************************************************************/
/*  text_button.cpp                                                      */
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
#include "text_button.hpp"
#include "container.hpp"
#include "gui_system.hpp"

namespace sun {

TextButton::TextButton(Context& context)
:   Widget(context),
    text_(context)
{}

void TextButton::draw(Renderer* renderer) const
{

}

void TextButton::handle_events(const Event& event)
{
}

void TextButton::on_parent_set_()
{
    text_.set_font(gui_->get_default_theme().font.get());
    text_.set_text("Button");
    bounds_ = dynamic_cast<Container*>(parent_)->request_bounds({
        0, 0,
        128, 32
    });
}

}
