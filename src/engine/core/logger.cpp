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

logger::logger() : level_(loglevel::info), show_timestamp_(true) {}

logger& logger::instance()
{
    static logger singleton;
    return singleton;
}

void logger::log(const std::string& message, loglevel level)
{
    if (level < level_) {
        return;
    }

    if (show_timestamp_)
    {
        std::printf("[%s ", get_timestamp_().c_str());

    } else {
        std::printf("[");
    }

    switch (level)
    {
        case loglevel::info:
            std::printf("\033[1;37mINFO\033[0m] ");
            break;
        case loglevel::debug:
            std::printf("\033[1;34mDEBUG\033[0m] ");
            break;
        case loglevel::warn:
            std::printf("\033[1;33mWARN\033[0m] ");
            break;
        case loglevel::error:
            std::printf("\033[1;31mERROR\033[0m] ");
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

    std::printf("%s\n", message.c_str());
}

void logger::naked_log(const std::string& message)
{
    std::printf("%s\n", message.c_str());
}

std::string logger::get_timestamp_()
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
