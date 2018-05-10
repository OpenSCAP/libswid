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
#include <map>
#include <regex>

#include "Translator.h"


using std::string;
using std::map;


const map<const char *, role_id> role_id_map = {
	{"aggregator", SWID_ROLE_AGGREGATOR},
	{"distributor", SWID_ROLE_DISTRIBUTOR},
	{"licensor", SWID_ROLE_LICENSOR},
	{"softwareCreator", SWID_ROLE_SOFTWARE_CREATOR},
	{"tagCreator", SWID_ROLE_TAG_CREATOR}
};


static void rtrim_string(string & str) {
	str.erase(str.find_last_not_of(" \n\r\t") + 1);
}


Role::Role():role(SWID_ROLE_NONE) {
}


Role::Role(string desc):role(SWID_ROLE_NONE) {
	for (auto it = role_id_map.begin(); it != role_id_map.end(); it++)
	{
		string regexp_text = "\\b";
		regexp_text = it->first;
		regexp_text += "\\b";
		auto regexp = std::regex(regexp_text);
		if (std::regex_search(desc, regexp)) {
			role = role | it->second;
		}
	}
}


Role::Role(role_id id): role(id) {
}


string Role::RoleAsString() const {
	auto ret = string();
	for (auto it = role_id_map.begin(); it != role_id_map.end(); it++)
	{
		if (role & it->second) {
			ret += it->first;
			ret += " ";
		}
	}

	rtrim_string(ret);
	return ret;
}


role_id Role::RoleAsId() const {
	return role;
}


const char * XML_TRUE = "true";
const char * XML_FALSE = "false";


static bool check_bool(const char * value) {
	if (strcmp(XML_TRUE, value) == 0) {
		return true;
	}
	return false;
}


type_id determine_type_id(const char * corpus, const char * patch, const char * supplemental) {
	type_id type = SWID_TYPE_PRIMARY;
	if (check_bool(corpus)) {
		type = SWID_TYPE_CORPUS;
	} else if (check_bool(patch)) {
		type = SWID_TYPE_PATCH;
	} else if (check_bool(supplemental)) {
		type = SWID_TYPE_SUPPLEMENTAL;
	}
	return type;
}


void set_strings_to_match_type(const type_id & type, string & corpus, string & patch, string & supplemental) {
	corpus = XML_FALSE;
	patch = XML_FALSE;
	supplemental = XML_FALSE;
	if (type == SWID_TYPE_CORPUS) {
		corpus = XML_TRUE;
	} else if (type == SWID_TYPE_PATCH) {
		patch = XML_TRUE;
	} else if (type == SWID_TYPE_SUPPLEMENTAL) {
		supplemental = XML_TRUE;
	}
}
