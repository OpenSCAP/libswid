#pragma once

#include "lib.h"

#include <tinyxml.h>


class TiXMLSWIDTagIO: public SWIDTagIO
{
public:
	TiXMLSWIDTagIO();
	virtual ~TiXMLSWIDTagIO() override;
	virtual SWIDStruct load(const std::string & filename) override;
	virtual void save(const std::string & filename, const SWIDStruct & what) override;
};
