/*************************************************************************/
/*  resource_cache.hpp                                                   */
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

#include "core/context.hpp"
#include "system/system.hpp"
#include "graphics/renderer.hpp"

#include <string>
#include <memory>
#include <unordered_map>

namespace sun {

class Resource;

class SUN_API ResourceCache final : public System
{
public:

    SUN_SYSTEM_TYPE(ResourceCache)

    explicit ResourceCache(Context&);

    bool init() override;

    void shutdown() override;

    template<typename T>
    std::shared_ptr<T> get_resource(const std::string& name) {
        auto res_name = T::get_static_type_name() + ":" + name;
        auto it = resources_.find(res_name);

        if (it != resources_.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        else {
            T* res = context_.create_resource<T>();

            if (res->load(path_ + name)) {
                auto res_shared_ptr = std::shared_ptr<T>(res);
                resources_[res_name] = static_cast<std::shared_ptr<Resource>>
                    (res_shared_ptr);
                return res_shared_ptr;
            } else {
                delete res;
                return nullptr;
            }
        }
    }

    inline void set_path(const std::string& path) {
        path_ = path;
        if (path_[path_.length() - 1] != '/') {
            path_.push_back('/');
        }
    }

    inline const std::string& get_path() const {
        return path_;
    }



    Component* create_component_(uint type_hash, uint id) override {
        return nullptr;
    }

    bool handles_component_(uint type_hash) override {
        return false;
    }

private:

    std::string path_;
    std::unordered_map<std::string, std::shared_ptr<Resource>>  resources_;
};

}
