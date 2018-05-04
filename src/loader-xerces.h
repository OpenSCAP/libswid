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

	xercesc::XercesDOMParser * parser;
	xercesc::ErrorHandler * errHandler;
};
