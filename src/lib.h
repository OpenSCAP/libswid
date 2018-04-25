#pragma once

#include <string>

using std::string;


class SWIDTagLoader
{
public:
	SWIDTagLoader();
	virtual ~SWIDTagLoader();
	virtual void load(const string & filename) = 0;
};
