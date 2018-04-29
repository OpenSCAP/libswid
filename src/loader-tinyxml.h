#pragma once

#include "lib.h"

#include <tinyxml.h>


class TiXML2SWIDTagLoader: public SWIDTagLoader
{
public:
	TiXML2SWIDTagLoader();
	virtual ~TiXML2SWIDTagLoader() override;
	virtual SWIDStruct load(const string & filename) override;
	virtual void save(const string & filename, const SWIDStruct & what) override;

private:
	const char * bool_to_string(bool) const;
	bool check_bool(const char *) const;
};
