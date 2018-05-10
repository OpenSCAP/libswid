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
#include <vector>


/**
 * SWID Tag Types
 *
 * A tag may have exactly one type.
 */
enum type_id {SWID_TYPE_PRIMARY, SWID_TYPE_CORPUS, SWID_TYPE_PATCH, SWID_TYPE_SUPPLEMENTAL};


/**
 * SWID Entity Roles
 *
 * An entity has at least one role. More than one role at the same time are allowed.
 */
enum role_id {
	SWID_ROLE_NONE = 0,
	SWID_ROLE_AGGREGATOR
		= 1 << 0,
	SWID_ROLE_DISTRIBUTOR
		= 1 << 1,
	SWID_ROLE_LICENSOR
		= 1 << 2,
	SWID_ROLE_SOFTWARE_CREATOR
		= 1 << 3,
	SWID_ROLE_TAG_CREATOR
		= 1 << 4
};


role_id operator | (role_id a, role_id b);
role_id operator & (role_id a, role_id b);


class SWIDEntity
{
public:
	std::string name;
	std::string regid;
	role_id role;

	bool operator == (const SWIDEntity & rhs) const;
};
typedef std::vector<SWIDEntity> Entities;


class SWIDLink
{
public:
	std::string href;
	std::string rel;
	bool operator == (const SWIDLink & rhs) const;
};
typedef std::vector<SWIDLink> Links;


class SWIDBase
{
public:
	SWIDBase();

	/// Set all empty values to their defaults.
	void applyDefaults();

	/// Name of the product or component as it would normally be referenced. HARD REQUIREMENT.
	std::string name;
	/// Globally unique identifier of the tag. HARD REQUIREMENT.
	std::string tagId;
	/// Detailed version of the product. SOFT REQUIREMENT, defaults to "0.0"
	std::string version;
	/// Encoding of the version, e.g. "semver", "decimal", "unknown". SOFT REQUIREMENT, defaults to "multipartnumeric"
	std::string versionScheme;
	/// Language of the tag in form of W3C-langtag. Automatically inherited to sub-elements. SOFT REQUIREMENT without default.
	std::string xml_lang;
	/// OPTIONAL, defaults to SWID_TYPE_PRIMARY.
	type_id type;
};


/**
 * The SWID Tag as a data structure.
 */
class SWIDStruct : public SWIDBase
{
public:
	SWIDStruct();

	/// Ordered list of entities
	Entities entities;
	/// Ordered list of links
	Links links;
};
