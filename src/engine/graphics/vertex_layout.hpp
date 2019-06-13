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
#include <vector>

namespace sun {

enum class vertex_data_type : uint8
{
    none = 0,
    float1,
    float2,
    float3,
    float4,
    int1,
    int2,
    int3,
    int4,
    boolean,
    mat3,
    mat4
};

static constexpr uint32 get_data_type_size(vertex_data_type type)
{
    switch (type)
    {
        case vertex_data_type::none:    return 0;
        case vertex_data_type::float1:  return 4;
        case vertex_data_type::float2:  return 4 * 2;
        case vertex_data_type::float3:  return 4 * 3;
        case vertex_data_type::float4:  return 4 * 4;
        case vertex_data_type::int1:    return 4;
        case vertex_data_type::int2:    return 4 * 2;
        case vertex_data_type::int3:    return 4 * 3;
        case vertex_data_type::int4:    return 4 * 4;
        case vertex_data_type::boolean: return 1;
        case vertex_data_type::mat3:    return 4 * 3 * 3;
        case vertex_data_type::mat4:    return 4 * 4 * 4;
    }
    return 0;
}

struct vertex_element
{
    vertex_data_type    type;
    uint32              size;
    uint32              offset;
    bool                normalized;

    vertex_element(vertex_data_type p_type, bool p_normalized = false)
    :   type(p_type),
        size(get_data_type_size(p_type)),
        offset(0),
        normalized(p_normalized)
    {}

    uint32 get_component_count() const
    {
        switch(type)
        {
            case vertex_data_type::none:    return 0;
            case vertex_data_type::float1:  return 1;
            case vertex_data_type::float2:  return 2;
            case vertex_data_type::float3:  return 3;
            case vertex_data_type::float4:  return 4;
            case vertex_data_type::int1:    return 1;
            case vertex_data_type::int2:    return 2;
            case vertex_data_type::int3:    return 3;
            case vertex_data_type::int4:    return 4;
            case vertex_data_type::boolean: return 1;
            case vertex_data_type::mat3:    return 3 * 3;
            case vertex_data_type::mat4:    return 4 * 4;
        }
    }
};

class vertex_layout
{
public:

    vertex_layout(const std::initializer_list<vertex_element>& elements)
    : elements_(elements)
    {
        calculate_offset_and_stride_();
    }

    std::vector<vertex_element>::iterator begin() { return elements_.begin(); }

    std::vector<vertex_element>::iterator end() { return elements_.end(); }

    inline const std::vector<vertex_element> get_elements() const {
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

    std::vector<vertex_element> elements_;
    uint32                      stride_;
};

}

