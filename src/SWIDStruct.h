#pragma once

#include <string>
#include <vector>

enum type_id {SWID_TYPE_PRIMARY, SWID_TYPE_CORPUS, SWID_TYPE_PATCH, SWID_TYPE_SUPPLEMENTAL};

enum role_id {
	SWID_ROLE_NONE = 0,
	SWID_ROLE_AGGREGATOR
		= 1 << 0,
	SWID_ROLE_DISTRIBUTOR
		= 1 << 1,
	SWID_ROLE_LICENSOR
		= 1 << 2,
	SWID_ROLE_SOFTWARE_CREATOR
		= 1 << 3,
	SWID_ROLE_TAG_CREATOR
		= 1 << 4
};

inline role_id operator | (role_id a, role_id b)
{return static_cast<role_id>(static_cast<int>(a) | static_cast<int>(b));}

inline role_id operator & (role_id a, role_id b)
{return static_cast<role_id>(static_cast<int>(a) & static_cast<int>(b));}


enum link_rel_id {SWID_LINK_REL_COMPONENT, SWID_LINK_REL_PATCHES, SWID_LINK_REL_REQUIRES, SWID_LINK_REL_SUPERSEDES, SWID_LINK_REL_SUPPLEMENTAL, SWID_LINK_REL_ANY};


class SWIDEntity
{
public:
	std::string name;
	role_id role;

	bool operator == (const SWIDEntity & rhs) const
	{
		return name == rhs.name && role == rhs.role;
	}
};
typedef std::vector<SWIDEntity> Entities;


class SWIDStruct
{
public:
	std::string name;
	std::string tagId;
	/// Default tag type is "primary"
	type_id type;

	Entities entities;
};


class SWIDLink
{
public:
	std::string href;
	link_rel_id relation;
};
