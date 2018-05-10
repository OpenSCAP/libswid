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

#include <map>
#include "lib.h"

#define SWID_NS "http://standards.iso.org/iso/19770/-2/2015/schema.xsd"


enum {SWID_ELEMENT_ENTITY = 0, SWID_ELEMENT_LINK, TOTAL_SWID_ELEMENT_COUNT};


XMLIOError create_read_error(const std::string & filename, const std::string & what_happened);
XMLIOError create_save_error(const std::string & filename, const std::string & what_happened);


template<class el_t>
class XMLIO : public SWIDTagIO
{
public:
	virtual ~XMLIO() override;

	virtual SWIDStruct load(const std::string & filename) override;
	virtual void save(const std::string & filename, const SWIDStruct & what) override;

protected:
	virtual void setAttrValue(el_t * el, const char * name, const std::string & value) = 0;
	virtual std::string extractAttrValue(el_t * el, const char * name) const = 0;
	virtual std::map<int, std::vector<el_t *> > subElementsOf(el_t * el) const = 0;
	virtual el_t * createRoot() = 0;
	virtual el_t * readRoot(const std::string & filename) = 0;
	virtual el_t * createSubElement(el_t * parent, int element_type) = 0;
	virtual void saveToFile(const std::string & filename) = 0;

	std::map<int, std::vector<el_t *> > createEmptyMap() const;
private:
	void addEntities(SWIDStruct & subject, const std::vector<el_t *> & entities);
	void addLinks(SWIDStruct & subject, const std::vector<el_t *> & links);
};
