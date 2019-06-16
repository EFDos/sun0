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
#include "logger.hpp"
#include <cstdio>
#include <ctime>

namespace sun {

Logger::Logger() : level_(LogLevel::All), show_timestamp_(true) {}

Logger& Logger::instance()
{
    static Logger singleton;
    return singleton;
}

void Logger::log(const std::string& message, LogLevel level)
{
    if (level < level_) {
        return;
    }

    auto output = (level == LogLevel::Error || level == LogLevel::Fatal) ?
             stderr : stdout;

    if (show_timestamp_)
    {
        std::fprintf(output, "[%s ", get_timestamp_().c_str());

    } else {
        std::fprintf(output, "[");
    }

    switch (level)
    {
        case LogLevel::All:
            return;
        case LogLevel::Trace:
            std::fprintf(output, "\033[1;37mTRACE\033[0m] ");
            break;
        case LogLevel::Debug:
            std::fprintf(output, "\033[1;34mDEBUG\033[0m] ");
            break;
        case LogLevel::Info:
            std::fprintf(output, "\033[1;37mINFO\033[0m] ");
            break;
        case LogLevel::Warn:
            std::fprintf(output, "\033[1;33mWARN\033[0m] ");
            break;
        case LogLevel::Error:
            std::fprintf(output, "\033[1;31mERROR\033[0m] ");
            break;
        case LogLevel::Fatal:
            std::fprintf(output, "\033[1;31mFATAL\033[0m] ");
            break;
    }

    /*if (!caller_info_.namespace_s.empty()) {
        std::printf("%s::", caller_info_.namespace_s.c_str());
    }

    if (!caller_info_.class_s.empty()) {
        std::printf("%s::", caller_info_.class_s.c_str());
    }

    if (!caller_info_.function_s.empty()) {
        std::printf("%s(): ", caller_info_.function_s.c_str());
    }*/

    std::fprintf(output, "%s\n", message.c_str());
}

void Logger::naked_log(const std::string& message)
{
    std::printf("%s\n", message.c_str());
}

std::string Logger::get_timestamp_()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 80, "%m-%d-%Y %I:%M:%S", timeinfo);

    return std::string(buffer);
}



}
