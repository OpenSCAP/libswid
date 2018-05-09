#pragma once

#include "lib.h"

#include <tinyxml.h>

#include "loader-generic.h"


class TiXMLSWIDTagIO : public XMLIO<TiXmlElement>
{
public:
	TiXMLSWIDTagIO();
private:
	virtual void setAttrValue(TiXmlElement * el, const char * name, const std::string & value) override;
	virtual std::string extractAttrValue(TiXmlElement * el, const char * name) const override;
	virtual std::map<int, std::vector<TiXmlElement *> > subElementsOf(TiXmlElement * el) const override;
	virtual TiXmlElement * createRoot() override;
	virtual TiXmlElement * readRoot(const std::string & filename) override;
	virtual TiXmlElement * createSubElement(TiXmlElement * parent, int element_type) override;
	virtual void saveToFile(const std::string & filename) override;

	const char * element_strings[TOTAL_SWID_ELEMENT_COUNT];

	TiXmlDocument doc;
};
