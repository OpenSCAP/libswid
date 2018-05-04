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


role_id operator | (role_id a, role_id b);
role_id operator & (role_id a, role_id b);


// enum link_rel_id {SWID_LINK_REL_COMPONENT, SWID_LINK_REL_PATCHES, SWID_LINK_REL_REQUIRES, SWID_LINK_REL_SUPERSEDES, SWID_LINK_REL_SUPPLEMENTAL, SWID_LINK_REL_ANY};


class SWIDEntity
{
public:
	std::string name;
	std::string regid;
	role_id role;

	bool operator == (const SWIDEntity & rhs) const;
};
typedef std::vector<SWIDEntity> Entities;


class SWIDLink
{
public:
	std::string href;
	std::string rel;
	bool operator == (const SWIDLink & rhs) const;
};
typedef std::vector<SWIDLink> Links;


class SWIDStruct
{
public:
	SWIDStruct();

	void applyDefaults();

	std::string name;
	std::string tagId;
	std::string version;
	std::string versionScheme;
	std::string xml_lang;
	/// Default tag type is "primary"
	type_id type;

	Entities entities;
	Links links;
};
