#pragma once

#include <string>

#include "SWIDStruct.h"


class Role
{
public:
	Role();
	Role(std::string);
	Role(role_id);

	std::string RoleAsString() const;
	role_id RoleAsId() const;
private:
	role_id role;
};
