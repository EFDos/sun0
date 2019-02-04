/*************************************************************************/
/*  source.cpp                                                           */
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
#include "filesys.hpp"
#include "core/logger.hpp"

#include <cstdio>
#include <cstring>

namespace sun {

namespace filesys {

std::string read_file(const std::string& path)
{
	FILE* file = fopen(path.c_str(), "rt");

	if (file != nullptr)
	{
		fseek(file, 0, SEEK_END);
		size_t len = ftell(file);

		char* buffer = new char[len + 1];
		memset(buffer, 0, len + 1);

		fseek(file, 0, SEEK_SET);
		fread(buffer, 1, len, file);

		std::string return_str(buffer);

		delete [] buffer;

		fclose(file);

		return return_str;
	} else {
		sun_logf_error("filesys::read_file: Could not open file: %s", path.c_str());
		return std::string();
	}
}

}

}
