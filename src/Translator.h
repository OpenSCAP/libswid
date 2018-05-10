/*
 * libswid, The SWID Tag Manipulation Library
 * Copyright (C) 2018, Red Hat Security Compliance Team.
 *
 * Red Hat, Inc., hereby disclaims all copyright interes
 * in the library 'libswid'.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#pragma once

#include <string>

#include "SWIDStruct.h"


class Role
{
public:
	Role();
	explicit Role(std::string);
	explicit Role(role_id);

	std::string RoleAsString() const;
	role_id RoleAsId() const;

private:
	role_id role;
};


type_id determine_type_id(
	 const char * corpus,  const char * patch,  const char * supplemental);

void set_strings_to_match_type(const type_id & type,
	std::string & corpus, std::string & patch, std::string & supplemental);
