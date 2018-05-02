#pragma once

#include <string>

#include "SWIDStruct.h"


class Role
{
public:
	Role();
	explicit Role(std::string);
	explicit Role(role_id);

	std::string RoleAsString() const;
	role_id RoleAsId() const;

private:
	role_id role;
};


type_id determine_type_id(
	 const char * corpus,  const char * patch,  const char * supplemental);

void set_strings_to_match_type(const type_id & type,
	std::string & corpus, std::string & patch, std::string & supplemental);
