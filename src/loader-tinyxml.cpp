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
#include "loader-tinyxml.h"
#include "Translator.h"

#include "loader-generic.cpp"

using std::string;


TiXMLSWIDTagIO::TiXMLSWIDTagIO()
{
	element_strings[SWID_ELEMENT_ENTITY] = "Entity";
	element_strings[SWID_ELEMENT_LINK] = "Link";
}


void TiXMLSWIDTagIO::setAttrValue(TiXmlElement * el, const char * name, const string & value)
{
	el->SetAttribute(name, value);
}


string TiXMLSWIDTagIO::extractAttrValue(TiXmlElement * el, const char * name) const
{
	return el->Attribute(name);
}


std::map<int, std::vector<TiXmlElement *> > TiXMLSWIDTagIO::subElementsOf(TiXmlElement * el) const
{
	auto result = createEmptyMap();
	for (auto * sub_el = el->FirstChildElement(); sub_el != NULL;
	     sub_el = sub_el->NextSiblingElement()) {
		for (int i = 0; i < TOTAL_SWID_ELEMENT_COUNT; i++) {
			if (strcmp(sub_el->Value(), element_strings[i]) == 0) {
				result[i].push_back(sub_el);
				break;
			}
		}
	}
	return result;
}


TiXmlElement * TiXMLSWIDTagIO::createRoot()
{
	doc = TiXmlDocument();

	auto * decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );
	auto * pRoot = new TiXmlElement("SoftwareIdentity");
	setAttrValue(pRoot, "xmlns", SWID_NS);
	doc.LinkEndChild( pRoot );
	return pRoot;
}


TiXmlElement * TiXMLSWIDTagIO::readRoot(const std::string & filename)
{
	doc = TiXmlDocument();

	auto result = doc.LoadFile(filename.c_str());
	if (! result) {
		throw create_read_error(filename, doc.ErrorDesc());
	}
	return doc.RootElement();
}


TiXmlElement * TiXMLSWIDTagIO::createSubElement(TiXmlElement * parent, int element_type)
{
	auto * entity_el = new TiXmlElement(element_strings[element_type]);
	parent->LinkEndChild(entity_el);
	return entity_el;
}


void TiXMLSWIDTagIO::saveToFile(const std::string & filename)
{
	auto result = doc.SaveFile(filename.c_str());
	if (! result) {
		throw create_save_error(filename, doc.ErrorDesc());
	}
}
