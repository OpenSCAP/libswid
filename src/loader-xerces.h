#pragma once

#include "lib.h"

#include <xercesc/parsers/XercesDOMParser.hpp>

using namespace xercesc;


class XercesSWIDTagLoader: public SWIDTagLoader
{
public:
	XercesSWIDTagLoader();
	virtual ~XercesSWIDTagLoader() override;
	virtual void load(const string & filename) override;

private:
	XercesDOMParser * parser;
};
