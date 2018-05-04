#pragma once

#include "lib.h"

#include <xercesc/parsers/XercesDOMParser.hpp>


class XercesSWIDTagIO: public SWIDTagIO
{
public:
	XercesSWIDTagIO();
	virtual ~XercesSWIDTagIO() override;
	virtual SWIDStruct load(const std::string & filename) override;
	virtual void save(const std::string & filename, const SWIDStruct & what) override;

private:
	void deleteParser();
	void createParser();

	void setAttrValue(xercesc::DOMElement * el, const char * name, const std::string & value);
	std::string extractAttrValue(xercesc::DOMElement * el, const char * name);

	xercesc::XercesDOMParser * parser;
	xercesc::ErrorHandler * errHandler;

	XMLCh * swid_ns;
	XMLCh * xmlch_entity;
	XMLCh * xmlch_link;
};
