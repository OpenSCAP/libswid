#pragma once

#include <map>

#include "lib.h"
#include "loader-generic.h"

#include <xercesc/parsers/XercesDOMParser.hpp>


class XercesSWIDTagIO : public XMLIO<xercesc::DOMElement>
{
public:
	XercesSWIDTagIO();
	virtual ~XercesSWIDTagIO() override;

	virtual SWIDStruct load(const std::string & filename) override;
	// virtual void save(const std::string & filename, const SWIDStruct & what) override;

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
