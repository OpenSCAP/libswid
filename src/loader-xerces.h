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

#include <map>

#include "libswid-base.h"
#include "loader-generic.h"

#include <xercesc/parsers/XercesDOMParser.hpp>


class XercesSWIDTagIO : public XMLIO<xercesc::DOMElement>
{
public:
	XercesSWIDTagIO();
	virtual ~XercesSWIDTagIO() override;

	virtual SWIDStruct load(const std::string & filename) override;
	virtual void save(const std::string & filename, const SWIDStruct & what) override;

	virtual validity is_xsd_valid(const std::string & filename) override;

private:
	void deleteParser();
	void createParser();

	void setAttrValue(xercesc::DOMElement * el, const char * name, const std::string & value) override;
	std::string extractAttrValue(xercesc::DOMElement * el, const char * name) const override;
	std::map<int, std::vector<xercesc::DOMElement *> > subElementsOf(xercesc::DOMElement * el) const override;

	xercesc::DOMElement * createRoot() override;
	xercesc::DOMElement * readRoot(const std::string & filename) override;

	xercesc::XercesDOMParser * parser;
	xercesc::ErrorHandler * errHandler;
	xercesc::DOMDocument * doc;

	xercesc::DOMElement * createSubElement(xercesc::DOMElement * root, int element_type) override;
	virtual void saveToFile(const std::string & filename) override;

	XMLCh * element_strings[TOTAL_SWID_ELEMENT_COUNT];
	XMLCh * swid_ns;
	XMLCh * xmlch_entity;
	XMLCh * xmlch_link;
};
