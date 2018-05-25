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

#include <sstream>

#include "libswid-base.h"

#ifdef HAVE_XERCES
#include "loader-xerces.h"
#define NEW_XERCES_IO new XercesSWIDTagIO()
#else
#define NEW_XERCES_IO NULL
#endif

#ifdef HAVE_TINYXML
#include "loader-tinyxml.h"
#define NEW_TINYXML_IO new TiXMLSWIDTagIO()
#else
#define NEW_TINYXML_IO NULL
#endif


XMLIOError::XMLIOError(const std::string & what_arg):std::runtime_error(what_arg) {
}


XMLIOError::XMLIOError(const char * what_arg):std::runtime_error(what_arg) {
}


SWIDTagIOBase::~SWIDTagIOBase() {
}


validity SWIDTagIOBase::is_xsd_valid(const std::string & filename) {
	return SWID_VALIDITY_UNKNOWN;
}


static SWIDTagIOBase * get_swidtagio(const char * type) {
	SWIDTagIOBase * ret = nullptr;
	if (strcmp(type, "tinyxml") == 0) {
		ret = NEW_TINYXML_IO;
	} else if (strcmp(type, "xerces") == 0) {
		ret = NEW_XERCES_IO;
	} else {
		throw std::runtime_error("Backend not known");
	}
	if (ret == nullptr) {
		throw std::runtime_error("Backend known, but not supported.");
	}
	return ret;
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
		current_backend = "";
	}
	try {
	    backend = get_swidtagio(backend_name.c_str());
	} catch (const std::runtime_error & exc) {
		std::ostringstream msg;
		msg << "Unable to set backend to '"
			<< backend_name << "': "
			<< exc.what() << ".\n";
		throw std::runtime_error(msg.str());
	}
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


validity SWIDTagIO::is_xsd_valid(const std::string & filename) {
	return backend->is_xsd_valid(filename);
}
