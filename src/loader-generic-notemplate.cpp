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
#include <sstream>

#include "loader-generic.h"

using std::ostringstream;


static XMLIOError create_xml_io_error(const std::string & filename, const std::string & error_intro, const std::string & what_happened) {
	ostringstream msg;
	msg << error_intro;
	msg << " '";
	msg << filename;
	msg << "': ";
	msg << what_happened;
	return XMLIOError(msg.str());
}


XMLIOError create_read_error(const std::string & filename, const std::string & what_happened) {
	return create_xml_io_error(filename, "Could not load from", what_happened);
}


XMLIOError create_save_error(const std::string & filename, const std::string & what_happened) {
	return create_xml_io_error(filename, "Could not save to", what_happened);
}
