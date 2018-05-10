/*
 * libswid, The SWID Tag Manipulation Library
 * Copyright (C) 2018, Red Hat Security Compliance Team.
 *
 * Red Hat, Inc., Durham, North Carolina hereby disclaims
 * all copyright interest in the library 'libswid'.
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

#include <stdexcept>
#include <string>

#include "SWIDStruct.h"



class XMLIOError : public std::runtime_error
{
public:
	explicit XMLIOError(const std::string & what_arg);
	explicit XMLIOError(const char * what_arg);
};


class SWIDTagIO
{
public:
	virtual ~SWIDTagIO();

	/**
	 * Get a SWIDStruct instance from an XML file.
	 */
	virtual SWIDStruct load(const std::string & filename) = 0;
	/**
	 * Save a SWIDStruct instance to an XML file.
	 */
	virtual void save(const std::string & filename, const SWIDStruct & what) = 0;
};


/**
 * Get pointer to a SWIDTagIO instance.
 *
 * Remember to free it using `delete`!
 *
 * Args:
 *  - type: The type string. May be one of "xerces", "tinyxml"
 */
SWIDTagIO * get_a_swidtagio(const char * type);
