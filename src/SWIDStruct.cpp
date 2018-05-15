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
#include "SWIDStruct.h"


// Took from https://stackoverflow.com/a/1448478/592892
role_id operator | (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) | static_cast<int>(b));
}


role_id operator & (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) & static_cast<int>(b));
}


void SWIDStruct::applyDefaults() {
	if (versionScheme.size() == 0) {
		versionScheme = "multipartnumeric";
	}
	if (version.size() == 0) {
		version = "0.0";
	}
}


SWIDStruct::SWIDStruct():
	name(), tagId(), version(), versionScheme(), xml_lang(), type(SWID_TYPE_PRIMARY),
	entities(), links()
{
}


bool SWIDEntity::operator == (const SWIDEntity & rhs) const
{
	return name == rhs.name && role == rhs.role && regid == rhs.regid;
}


bool SWIDLink::operator == (const SWIDLink & rhs) const
{
	return href == rhs.href && rel == rhs.rel;
}
