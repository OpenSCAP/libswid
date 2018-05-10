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
#include "loader-generic.h"
#include "Translator.h"

template<class el_t>
XMLIO<el_t>::~XMLIO() {
}


template<class el_t>
SWIDStruct XMLIO<el_t>::load(const std::string & filename) {
	el_t * pRoot = readRoot(filename);

	auto ret = SWIDStruct();

	ret.name = extractAttrValue(pRoot, "name");
	ret.tagId = extractAttrValue(pRoot, "tagId");
	ret.version = extractAttrValue(pRoot, "version");
	ret.versionScheme = extractAttrValue(pRoot, "versionScheme");
	ret.xml_lang = extractAttrValue(pRoot, "xml:lang");

	ret.type = determine_type_id(
		extractAttrValue(pRoot, "corpus").c_str(),
		extractAttrValue(pRoot, "patch").c_str(),
		extractAttrValue(pRoot, "supplemental").c_str());

	auto subelements = subElementsOf(pRoot);

	if (subelements.find(SWID_ELEMENT_ENTITY) != subelements.end()) {
		addEntities(ret, subelements[SWID_ELEMENT_ENTITY]);
	}

	if (subelements.find(SWID_ELEMENT_LINK) != subelements.end()) {
		addLinks(ret, subelements[SWID_ELEMENT_LINK]);
	}

	return ret;
}


template<class el_t>
void XMLIO<el_t>::addEntities(SWIDStruct & subject, const std::vector<el_t *> & entities) {
	std::string role;
	for (auto it = entities.begin(); it != entities.end(); it++) {
		auto entity = SWIDEntity();
		entity.name = extractAttrValue(* it, "name");
		entity.regid = extractAttrValue(* it, "regid");
		role = extractAttrValue(* it, "role");
		entity.role = Role(role).RoleAsId();
		subject.entities.push_back(entity);
	}
}


template<class el_t>
void XMLIO<el_t>::addLinks(SWIDStruct & subject, const std::vector<el_t *> & links) {
	for (auto it = links.begin(); it != links.end(); it++) {
		auto link = SWIDLink();
		link.href = extractAttrValue(* it, "href");
		link.rel = extractAttrValue(* it, "rel");
		subject.links.push_back(link);
	}
}


template<class el_t>
void XMLIO<el_t>::save(const std::string & filename, const SWIDStruct & what) {
	el_t * pRoot = createRoot();

	setAttrValue(pRoot, "name", what.name);
	setAttrValue(pRoot, "tagId", what.tagId);
	setAttrValue(pRoot, "version", what.version);
	setAttrValue(pRoot, "versionScheme", what.versionScheme);
	setAttrValue(pRoot, "xml:lang", what.xml_lang);

	std::string corpus, patch, supplemental;
	set_strings_to_match_type(what.type, corpus, patch, supplemental);
	setAttrValue(pRoot, "corpus", corpus);
	setAttrValue(pRoot, "patch", patch);
	setAttrValue(pRoot, "supplemental", supplemental);

	for (auto it = what.entities.begin(); it != what.entities.end(); it++) {
		el_t * el = createSubElement(pRoot, SWID_ELEMENT_ENTITY);
		setAttrValue(el, "name", it->name);
		setAttrValue(el, "regid", it->regid);
		setAttrValue(el, "role", Role(it->role).RoleAsString());
	}

	for (auto it = what.links.begin(); it != what.links.end(); it++) {
		el_t * el = createSubElement(pRoot, SWID_ELEMENT_LINK);
		setAttrValue(el, "href", it->href);
		setAttrValue(el, "rel", it->rel);
	}
	saveToFile(filename);
}


template<class el_t>
std::map<int, std::vector<el_t *> > XMLIO<el_t>::createEmptyMap() const {
	std::map<int, std::vector<el_t *> > result;
	for (int i = 0; i < TOTAL_SWID_ELEMENT_COUNT; i++) {
		result[i] = std::vector<el_t *>();
	}
	return result;
}
