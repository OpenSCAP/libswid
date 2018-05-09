#pragma once

#include "lib.h"

#include <tinyxml.h>

#include "loader-generic.h"


class TiXMLSWIDTagIO : public XMLIO<TiXmlElement>
{
public:
	TiXMLSWIDTagIO();
private:
	virtual void setAttrValue(TiXmlElement * el, const char * name, const std::string & value);
	virtual std::string extractAttrValue(TiXmlElement * el, const char * name) const;
	virtual std::map<int, std::vector<TiXmlElement *> > subElementsOf(TiXmlElement * el) const;
	virtual TiXmlElement * createRoot();
	virtual TiXmlElement * readRoot(const std::string & filename);
	virtual TiXmlElement * createSubElement(TiXmlElement * parent, int element_type);
	virtual void saveToFile(const std::string & filename);

	const char * element_strings[TOTAL_SWID_ELEMENT_COUNT];

	TiXmlDocument doc;
};
