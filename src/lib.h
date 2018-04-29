#pragma once

#include <string>

#include "SWIDStruct.h"

using std::string;


class SWIDTagLoader
{
public:
	SWIDTagLoader();
	virtual ~SWIDTagLoader();
	virtual SWIDStruct load(const string & filename) = 0;
	virtual void save(const string & filename, const SWIDStruct & what) = 0;
};
