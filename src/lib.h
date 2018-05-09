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


XMLReadError create_read_error(const std::string & filename, const std::string & what_happened);
XMLReadError create_save_error(const std::string & filename, const std::string & what_happened);


class SWIDTagIO
{
public:
	/**
	 * Get a SWIDStruct instance from an XML file.
	 */
	virtual SWIDStruct load(const std::string & filename) = 0;
	/**
	 * Save a SWIDStruct instance to an XML file.
	 */
	virtual void save(const std::string & filename, const SWIDStruct & what) = 0;
};


/**
 * Get pointer to a SWIDTagIO instance.
 *
 * Remember to free it using `delete`!
 *
 * Args:
 *  - type: The type string. May be one of "xerces", "tinyxml"
 */
SWIDTagIO * get_a_swidtagio(const char * type);
