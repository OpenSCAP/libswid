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

// TODO: Make a script that inserts and updates the copyright claim in cpp files

#include "lib.h"

#include "loader-xerces.h"
#include "loader-tinyxml.h"


XMLIOError::XMLIOError(const std::string & what_arg):std::runtime_error(what_arg) {
}


XMLIOError::XMLIOError(const char * what_arg):std::runtime_error(what_arg) {
}


SWIDTagIOBase::~SWIDTagIOBase() {
}


static SWIDTagIOBase * get_swidtagio(const char * type) {
	if (strcmp(type, "tinyxml") == 0) {
		return new TiXMLSWIDTagIO();
	} else if (strcmp(type, "xerces") == 0) {
		return new XercesSWIDTagIO();
	} else {
		std::ostringstream msg;
		msg << "Unable to set backend to '"
			<< type << "': Backend not known.";
		throw std::runtime_error(msg.str());
	}
}


SWIDTagIO::SWIDTagIO():SWIDTagIOBase(), current_backend(), backend(nullptr) {
}


SWIDTagIO::~SWIDTagIO() {
	delete backend;
	backend = nullptr;
	current_backend = "";
}


void SWIDTagIO::setBackend(const std::string & backend_name) {
	if (backend != nullptr) {
		delete backend;
	}
	backend = get_swidtagio(backend_name.c_str());
	current_backend = backend_name;
}


SWIDStruct SWIDTagIO::load(const std::string & filename) {
	if (backend == nullptr) {
		throw std::runtime_error("No backend has been set, call setBackend first.");
	}
	return backend->load(filename);
}


void SWIDTagIO::save(const std::string & filename, const SWIDStruct & what) {
	if (backend == nullptr) {
		throw std::runtime_error("No backend has been set, call setBackend first.");
	}
	return backend->save(filename, what);
}
