#pragma once

#include "lib.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

using namespace xercesc;


class XercesSWIDTagLoader: public SWIDTagLoader
{
public:
	XercesSWIDTagLoader();
	virtual ~XercesSWIDTagLoader() override;
	virtual SWIDStruct load(const string & filename) override;
	virtual void save(const string & filename, const SWIDStruct & what) override;

private:
	void deleteParser();
	void createParser();

	XercesDOMParser * parser;
};
