#pragma once

#include <stdexcept>
#include <string>

#include "SWIDStruct.h"


class XMLReadError : public std::runtime_error
{
public:
	explicit XMLReadError(const std::string & what_arg);
	explicit XMLReadError(const char * what_arg);
};


class SWIDTagIO
{
public:
	SWIDTagIO();
	virtual ~SWIDTagIO();
	virtual SWIDStruct load(const std::string & filename) = 0;
	virtual void save(const std::string & filename, const SWIDStruct & what) = 0;
};


SWIDTagIO * get_a_swidtagio(const char * type);
