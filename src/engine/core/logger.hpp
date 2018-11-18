/*************************************************************************/
/*  logger.hpp                                                           */
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
#include <string>
#include <memory>

namespace sun {

namespace string_utils {
template<typename... Args>
std::string format(const std::string& format, Args... args)
{
    // Extra space for '\0'
    size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1;
    std::unique_ptr<char[]> buf(new char[ size ]);
    snprintf(buf.get(), size, format.c_str(), args...);

    // We don't want the '\0' inside
    return std::string(buf.get(), buf.get() + size - 1);
}
} // string_utils

enum class loglevel {
    all,
    trace,
    debug,
    info,
    warn,
    error,
    fatal
};

class logger
{
public:

    void log(const std::string& message, loglevel level = loglevel::trace);

    void naked_log(const std::string& message);

    inline void set_loglevel(loglevel level) { level_ = level; }

    inline void set_show_timestamp(bool show) { show_timestamp_ = show; }

    static logger& instance();

private:

    loglevel    level_;
    bool        show_timestamp_;

    logger();

    logger(const logger&) = delete;

    logger(logger&&) = delete;

    std::string get_timestamp_();
};

} // sun

#define sun_print(m) sun::logger::instance().naked_log(m)
#define sun_printf(m, ...) sun::logger::instance().naked_log( \
    sun::string_utils::format(m, ##__VA_ARGS__))

#define sun_log(m)   sun::logger::instance().log(m)
#define sun_log_debug(m) sun::logger::instance().log(m, sun::loglevel::debug)
#define sun_log_info(m) sun::logger::instance().log(m, sun::loglevel::info)
#define sun_log_warn(m) sun::logger::instance().log(m, sun::loglevel::warn)
#define sun_log_error(m) sun::logger::instance().log(m, sun::loglevel::error)
#define sun_log_fatal(m) sun::logger::instance().log(m, sun::loglevel::fatal)

#define sun_logf(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__))
#define sun_logf_debug(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__), sun::loglevel::debug)
#define sun_logf_info(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__), sun::loglevel::info)
#define sun_logf_warn(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__), sun::loglevel::warn)
#define sun_logf_error(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__), sun::loglevel::error)
#define sun_logf_fatal(m, ...) sun::logger::instance().log( \
    sun::string_utils::format(m, ##__VA_ARGS__), sun::loglevel::fatal)
