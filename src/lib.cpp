/*
 * libswid, The SWID Tag Manipulation Library
 * Copyright (C) 2018, Red Hat Security Compliance Team.
 *
 * Red Hat, Inc., hereby disclaims all copyright interes
 * in the library `libswid'.
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

// TODO: Make a script that inserts and updates the copyright claim in cpp files

#include <sstream>

#include "lib.h"

#include "loader-xerces.h"
#include "loader-tinyxml.h"


using std::ostringstream;


XMLReadError::XMLReadError(const std::string & what_arg):std::runtime_error(what_arg) {
}


XMLReadError::XMLReadError(const char * what_arg):std::runtime_error(what_arg) {
}


XMLReadError create_read_error(const std::string & filename, const std::string & what_happened) {
	ostringstream msg;
	msg << "Error loading from '";
	msg << filename;
	msg << "': ";
	msg << what_happened;
	return XMLReadError(msg.str());
}


XMLReadError create_save_error(const std::string & filename, const std::string & what_happened) {
	ostringstream msg;
	msg << "Error saving to '";
	msg << filename;
	msg << "': ";
	msg << what_happened;
	return XMLReadError(msg.str());
}


SWIDTagIO * get_a_swidtagio(const char * type) {
	if (strcmp(type, "tinyxml") == 0) {
		return new TiXMLSWIDTagIO();
	} else if (strcmp(type, "xerces") == 0) {
		return new XercesSWIDTagIO();
	}
	return NULL;
}
