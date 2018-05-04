#include "SWIDStruct.h"


// Took from https://stackoverflow.com/a/1448478/592892
role_id operator | (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) | static_cast<int>(b));
}


role_id operator & (role_id a, role_id b) {
	return static_cast<role_id>(static_cast<int>(a) & static_cast<int>(b));
}


SWIDStruct::SWIDStruct():
	name(), tagId(), version(), versionScheme(), xml_lang(), type(SWID_TYPE_PRIMARY),
	entities()
{
}


void SWIDStruct::applyDefaults() {
	if (versionScheme.size() == 0) {
		versionScheme = "multipartnumeric";
	}
}


bool SWIDEntity::operator == (const SWIDEntity & rhs) const
{
	return name == rhs.name && role == rhs.role && regid == rhs.regid;
}


bool SWIDLink::operator == (const SWIDLink & rhs) const
{
	return href == rhs.href && rel == rhs.rel;
}
