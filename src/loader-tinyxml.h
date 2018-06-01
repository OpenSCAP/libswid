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

#include "libswid-base.h"

#include <tinyxml.h>

#include "loader-generic.h"


class TiXMLSWIDTagIO : public XMLIO<TiXmlElement>
{
public:
	TiXMLSWIDTagIO();
private:
	virtual void setAttrValue(TiXmlElement * el, const char * name,
	                          const std::string & value) override;
	virtual std::string extractAttrValue(TiXmlElement * el, const char * name) const override;
	virtual std::map<int, std::vector<TiXmlElement *> > subElementsOf(TiXmlElement * el) const
	override;
	virtual TiXmlElement * createRoot() override;
	virtual TiXmlElement * readRoot(const std::string & filename) override;
	virtual TiXmlElement * createSubElement(TiXmlElement * parent, int element_type) override;
	virtual void saveToFile(const std::string & filename) override;

	const char * element_strings[TOTAL_SWID_ELEMENT_COUNT];

	TiXmlDocument doc;
};
