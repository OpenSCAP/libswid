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


class SWIDTagIOBase
{
public:
	virtual ~SWIDTagIOBase();

	/**
	 * Get a SWIDStruct instance from an XML file.
	 */
	virtual SWIDStruct load(const std::string & filename) = 0;
	/**
	 * Save a SWIDStruct instance to an XML file.
	 */
	virtual void save(const std::string & filename, const SWIDStruct & what) = 0;
	/**
	 * Assess schema validity of the file.
	 */
	virtual validity is_xsd_valid(const std::string & filename);
};


class SWIDTagIO : public SWIDTagIOBase
{
public:
	SWIDTagIO();
	virtual ~SWIDTagIO() override;

	void setBackend(const std::string & backend_name);

	SWIDStruct load(const std::string & filename) override;
	void save(const std::string & filename, const SWIDStruct & what) override;
	virtual validity is_xsd_valid(const std::string & filename) override;

private:
	std::string current_backend;
	SWIDTagIOBase * backend;
};
