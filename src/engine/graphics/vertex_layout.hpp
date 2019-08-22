/*************************************************************************/
/*  vertex_layout.hpp                                                    */
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

#include "common/int.hpp"

namespace sun {

enum class VertexDataType : uint8
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool,
    Mat3,
    Mat4
};

static constexpr uint32 get_data_type_size(VertexDataType type)
{
    switch (type)
    {
        case VertexDataType::None:    return 0;
        case VertexDataType::Float:   return 4;
        case VertexDataType::Float2:  return 4 * 2;
        case VertexDataType::Float3:  return 4 * 3;
        case VertexDataType::Float4:  return 4 * 4;
        case VertexDataType::Int:     return 4;
        case VertexDataType::Int2:    return 4 * 2;
        case VertexDataType::Int3:    return 4 * 3;
        case VertexDataType::Int4:    return 4 * 4;
        case VertexDataType::Bool:    return 1;
        case VertexDataType::Mat3:    return 4 * 3 * 3;
        case VertexDataType::Mat4:    return 4 * 4 * 4;
    }
    return 0;
}

struct VertexElement
{
    VertexDataType    type;
    uint32              size;
    uint32              offset;
    bool                normalized;

    VertexElement(VertexDataType p_type, bool p_normalized = false)
    :   type(p_type),
        size(get_data_type_size(p_type)),
        offset(0),
        normalized(p_normalized)
    {}

    uint32 get_component_count() const
    {
        switch(type)
        {
            case VertexDataType::None:    return 0;
            case VertexDataType::Float:   return 1;
            case VertexDataType::Float2:  return 2;
            case VertexDataType::Float3:  return 3;
            case VertexDataType::Float4:  return 4;
            case VertexDataType::Int:     return 1;
            case VertexDataType::Int2:    return 2;
            case VertexDataType::Int3:    return 3;
            case VertexDataType::Int4:    return 4;
            case VertexDataType::Bool:    return 1;
            case VertexDataType::Mat3:    return 3 * 3;
            case VertexDataType::Mat4:    return 4 * 4;
        }
    }
};

class VertexLayout
{
public:

    VertexLayout(const std::initializer_list<VertexElement>& elements)
    : elements_(elements)
    {
        calculate_offset_and_stride_();
    }

    std::vector<VertexElement>::iterator begin() { return elements_.begin(); }

    std::vector<VertexElement>::iterator end() { return elements_.end(); }

    inline const std::vector<VertexElement> get_elements() const {
        return elements_;
    }

    inline uint32 get_stride() const {
        return stride_;
    }

private:

    void calculate_offset_and_stride_()
    {
        uint32 offset = 0;
        stride_ = 0;
        for (auto& element : elements_) {
            element.offset = offset;
            offset += element.size;
            stride_ += element.size;
        }
    }

    std::vector<VertexElement> elements_;
    uint32                      stride_;
};

}

